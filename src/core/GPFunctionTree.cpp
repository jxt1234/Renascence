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

void GPFunctionTreePoint::valid() const
{
    if (NULL!=mF)
    {
        for (size_t i=0; i<mChildren.size(); ++i)
        {
            auto pp = GPCONVERT(GPFunctionTreePoint, mChildren[i]);
            if (NULL!=pp->function())
            {
                GPASSERT(pp->function()->outputType[0] == mF->inputType[i]);
                pp->valid();
            }
        }
    }
}

int GPFunctionTreePoint::maxInputPos() const
{
    if (NULL==mF)
    {
        return mInputNumber;
    }
    int maxPos = -1;
    for (size_t i=0; i<mChildren.size(); ++i)
    {
        auto pp = GPCONVERT(GPFunctionTreePoint, mChildren[i]);
        int cur = pp->maxInputPos();
        if (cur > maxPos)
        {
            maxPos = cur;
        }
    }
    return maxPos;
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

void GPFunctionTreePoint::mapInput(const std::map<int, GPFunctionTreePoint*>& inputMap)
{
    for (size_t i=0; i<mChildren.size(); ++i)
    {
        auto pp = GPCONVERT(GPFunctionTreePoint, mChildren[i]);
        if (NULL == pp->function())
        {
            if (inputMap.find(pp->mInputNumber)!=inputMap.end())
            {
                mChildren[i] = inputMap.find(pp->mInputNumber)->second;
                mChildren[i]->addRef();
                pp->decRef();
            }
        }
        else
        {
            pp->mapInput(inputMap);
        }
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


std::map<int, const IStatusType*> GPFunctionTreePoint::getInputTypes() const
{
    std::map<int, const IStatusType*> result;
    _getInputTypes(result);
    /*Check Valid*/
    for (auto t : result)
    {
        GPASSERT(NULL!=t.second);
    }
    return result;
}
void GPFunctionTreePoint::_getInputTypes(std::map<int, const IStatusType*>& types) const
{
    GPASSERT(NULL!=mF);
    for (size_t i=0; i<mChildren.size(); ++i)
    {
        auto p = GPCONVERT(GPFunctionTreePoint, mChildren[i]);
        if (p->inputNumber()>=0)
        {
            if (types.find(p->inputNumber())!=types.end())
            {
                GPASSERT(types.find(p->inputNumber())->second == mF->inputType[i]);
            }
            else
            {
                types.insert(std::make_pair(p->inputNumber(), mF->inputType[i]));
            }
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
        std::vector<GPFunctionTreePoint*> immutable;
        mVariableSubTree.insert(std::make_pair(root.get(), immutable));
    }
    root->valid();
}
GPFunctionTree::~GPFunctionTree()
{
}


void GPFunctionTree::addVariableSubTree(GPFunctionTreePoint* subtree, const std::vector<GPFunctionTreePoint*>& immutable)
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
        for (auto pp : p.first->display())
        {
            if (pp == subtree)
            {
                GPASSERT(0);//FIXME
                return;
            }
        }
    }
    mVariableSubTree.insert(std::make_pair(subtree, immutable));
}


class GPFunctionTreePointCopy:public GPAbstractPoint::IPointCopy
{
public:
    virtual GPAbstractPoint* copy(GPAbstractPoint* src, bool& needCopyChild)
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
