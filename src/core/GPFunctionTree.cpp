#include "core/GPFunctionTree.h"
#include <algorithm>
#include <sstream>
GPFunctionTreePoint::GPFunctionTreePoint(const GPFunctionDataBase::function* f, int inputNumber)
{
    GPASSERT(NULL != f || inputNumber >=0);
    mF = f;
    mInputNumber = inputNumber;
}
GPFunctionTreePoint::~GPFunctionTreePoint()
{
}
bool GPFunctionTreePoint::equal(const GPFunctionTreePoint* point) const
{
    GPASSERT(NULL!=point);
    if (depth() != point->depth())
    {
        return false;
    }
    bool thesame = (mF == point->mF && mInputNumber == point->mInputNumber);
    if (thesame)
    {
        GPASSERT(mChildren.size() == point->mChildren.size());
        for (size_t i=0; i<mChildren.size(); ++i)
        {
            auto pp = GPCONVERT(GPFunctionTreePoint, mChildren[i]);
            auto op = GPCONVERT(GPFunctionTreePoint, point->mChildren[i]);
            if (!pp->equal(op))
            {
                thesame = false;
                break;
            }
        }
    }
    return thesame;
}

void GPFunctionTreePoint::mapInput(const std::map<int, int>& inputMap)
{
    if (mInputNumber>=0)
    {
        if (inputMap.find(mInputNumber)!=inputMap.end())
        {
            mInputNumber = inputMap.find(mInputNumber)->second;
        }
        return;
    }
    for (auto p : mChildren)
    {
        auto pp = GPCONVERT(GPFunctionTreePoint, p);
        pp->mapInput(inputMap);
    }
}


size_t GPFunctionTreePoint::depth() const
{
    size_t depth = 1;
    size_t maxdepth = 0;
    for (auto p : mChildren)
    {
        auto pp = GPCONVERT(GPFunctionTreePoint, p);
        maxdepth = std::max(maxdepth, pp->depth());
    }
    return depth+maxdepth;
}


std::vector<const IStatusType*> GPFunctionTreePoint::getInputTypes() const
{
    std::vector<const IStatusType*> result;
    _getInputTypes(result);
    /*Check Valid*/
    for (auto t : result)
    {
        GPASSERT(NULL!=t);
    }
    return result;
}
void GPFunctionTreePoint::_getInputTypes(std::vector<const IStatusType*>& types) const
{
    GPASSERT(NULL!=mF);
    for (size_t i=0; i<mChildren.size(); ++i)
    {
        auto p = GPCONVERT(GPFunctionTreePoint, mChildren[i]);
        if (p->inputNumber()>=0)
        {
            if (types.size() <= p->inputNumber())
            {
                for (size_t j=types.size(); j<=p->inputNumber(); ++j)
                {
                    types.push_back(NULL);
                }
            }
            types[p->inputNumber()] = mF->inputType[i];
        }
        else
        {
            p->_getInputTypes(types);
        }
    }
}



GPFunctionTree::GPFunctionTree(GPPtr<GPFunctionTreePoint> root, bool totalVariable)
{
    GPASSERT(NULL!=root.get());
    mRoot = root;
    if (totalVariable)
    {
        mVariableSubTree.push_back(root.get());
    }
}
GPFunctionTree::~GPFunctionTree()
{
}


void GPFunctionTree::addVariableSubTree(GPFunctionTreePoint* subtree)
{
    /*TODO find other way to validate*/
    auto points = mRoot->display();
    bool issubtree = false;
    for (auto p : points)
    {
        if (p == subtree)
        {
            issubtree = true;
            break;
        }
    }
    GPASSERT(issubtree);
    for (auto p : mVariableSubTree)
    {
        for (auto pp : p->display())
        {
            if (pp == subtree)
            {
                GPASSERT(0);//FIXME
                return;
            }
        }
    }
    mVariableSubTree.push_back(subtree);
}

class GPFunctionTreePointCopy:public GPAbstractPoint::IPointCopy
{
public:
    virtual GPAbstractPoint* copy(GPAbstractPoint* src)
    {
        GPFunctionTreePoint* _src = GPCONVERT(GPFunctionTreePoint, src);
        GPFunctionTreePoint* res = new GPFunctionTreePoint(_src->function(), _src->inputNumber());
        return res;
    }
private:
};


GPFunctionTree* GPFunctionTree::copy(const GPFunctionTree* origin)
{
    GPFunctionTreePointCopy copyt;
    GPFunctionTreePoint* p = GPCONVERT(GPFunctionTreePoint, GPAbstractPoint::deepCopy(origin->root(), &copyt));
    return new GPFunctionTree(p);
}

GPFunctionTreePoint* GPFunctionTree::copy(const GPFunctionTreePoint* origin)
{
    GPFunctionTreePointCopy copyt;
    GPFunctionTreePoint* p = GPCONVERT(GPFunctionTreePoint, GPAbstractPoint::deepCopy((GPFunctionTreePoint*)origin, &copyt));
    return p;
}

void GPFunctionTreePoint::render(std::ostream& output) const
{
    if (NULL!=mF)
    {
        output << mF->name << "(";
        for (size_t i=0; i<mChildren.size()-1; ++i)
        {
            auto pp = (GPFunctionTreePoint*)mChildren[i];
            pp->render(output);
            output << ", ";
        }
        if (mChildren.size()>0)
        {
            auto pp = (GPFunctionTreePoint*)mChildren[mChildren.size()-1];
            pp->render(output);
            output << ")";
        }
    }
    else
    {
        output << "x"<<mInputNumber;
    }
}


std::string GPFunctionTree::dump() const
{
    std::ostringstream output;
    mRoot->render(output);
    return output.str();
}
