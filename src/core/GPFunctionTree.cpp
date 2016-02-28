/******************************************************************
 Copyright 2016, Jiang Xiao-tang
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 ******************************************************************/

#include "core/GPFunctionTree.h"
#include <algorithm>
#include <sstream>
GPFunctionTreePoint::GPFunctionTreePoint(const GPFunctionDataBase::function* f)
{
    GPASSERT(NULL != f);
    mData.pFunc = f;
    mType = FUNCTION;
}
GPFunctionTreePoint::GPFunctionTreePoint(int inputPos)
{
    GPASSERT(inputPos>=0);
    mData.iInput = inputPos;
    mType = INPUT;
}

GPFunctionTreePoint::GPFunctionTreePoint(const GPFunctionTreePoint& src)
{
    mData = src.mData;
    mType = src.mType;
}


void GPFunctionTreePoint::copyFrom(const GPFunctionTreePoint* src)
{
    shallowCopyChildren(src);
    mData = src->mData;
    mType = src->mType;
}

GPFunctionTreePoint::~GPFunctionTreePoint()
{
}

void GPFunctionTreePoint::valid() const
{
    if (FUNCTION == mType)
    {
        for (size_t i=0; i<mChildren.size(); ++i)
        {
            auto pp = GPCONVERT(GPFunctionTreePoint, mChildren[i]);
            if (FUNCTION == pp->type())
            {
                GPASSERT(pp->data().pFunc->outputType[0] == mData.pFunc->inputType[i]);
                pp->valid();
            }
        }
    }
}

int GPFunctionTreePoint::maxInputPos() const
{
    if (INPUT==mType)
    {
        return mData.iInput;
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

bool GPFunctionTreePoint::_theSame(DATA a, DATA b, TYPE t)
{
    bool res = true;
    switch (t)
    {
        case FUNCTION:
            res = a.pFunc == b.pFunc;
            break;
        case INPUT:
            res = a.iInput == b.iInput;
            break;
        default:
            break;
    }
    return res;
}


bool GPFunctionTreePoint::equal(const GPFunctionTreePoint* point) const
{
    GPASSERT(NULL!=point);
    if (depth() != point->depth())
    {
        return false;
    }
    bool thesame = (mType == point->type() && _theSame(mData, point->data(), mType));
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
    if (INPUT == mType)
    {
        if (inputMap.find(mData.iInput)!=inputMap.end())
        {
            mData.iInput = inputMap.find(mData.iInput)->second;
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
        if (INPUT == pp->type())
        {
            if (inputMap.find(pp->data().iInput)!=inputMap.end())
            {
                mChildren[i] = inputMap.find(pp->data().iInput)->second;
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
    GPASSERT(FUNCTION == mType);
    for (size_t i=0; i<mChildren.size(); ++i)
    {
        auto p = GPCONVERT(GPFunctionTreePoint, mChildren[i]);
        if (INPUT == p->type())
        {
            if (types.find(p->data().iInput)!=types.end())
            {
                GPASSERT(types.find(p->data().iInput)->second == mData.pFunc->inputType[i]);
            }
            else
            {
                types.insert(std::make_pair(p->data().iInput, mData.pFunc->inputType[i]));
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


void GPFunctionTree::addVariableSubTree(GPFunctionTreePoint* subtree, const std::vector<GPFunctionTreePoint*>& immutable, const std::string& name)
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
    mSubTreeName.insert(std::make_pair(name, subtree));
}


class GPFunctionTreePointCopy:public GPAbstractPoint::IPointCopy
{
public:
    GPFunctionTreePointCopy(const GPFunctionTree* basic) : mBasic(basic){}
    virtual ~GPFunctionTreePointCopy(){}
    
    std::map<GPFunctionTreePoint*, GPFunctionTreePoint*> pMapTable;
    virtual GPAbstractPoint* copy(GPAbstractPoint* src, bool& needCopyChild)
    {
        GPFunctionTreePoint* _src = GPCONVERT(GPFunctionTreePoint, src);
        GPFunctionTreePoint* res = new GPFunctionTreePoint(*_src);
        if (NULL == mBasic)
        {
            return res;
        }
        for (auto iter : mBasic->getVariable())
        {
            if (_src == iter.first)
            {
                pMapTable.insert(std::make_pair(_src, res));
                return res;
            }
            for (auto c : iter.second)
            {
                if (c == _src)
                {
                    pMapTable.insert(std::make_pair(_src, res));
                    return res;
                }
            }
        }
        return res;
    }
private:
    const GPFunctionTree* mBasic;
};


GPFunctionTree* GPFunctionTree::copy(const GPFunctionTree* origin)
{
    GPASSERT(NULL!=origin);
    GPFunctionTreePointCopy copyt(origin);
    GPFunctionTreePoint* p = GPCONVERT(GPFunctionTreePoint, GPAbstractPoint::deepCopy(origin->root(), &copyt));
    if (origin->mVariableSubTree.empty())
    {
        return new GPFunctionTree(p);
    }
    GPFunctionTree* newp = new GPFunctionTree(p, false);
    auto names = origin->getSubTreeName();
    for (auto iter : origin->getVariable())
    {
        GPFunctionTreePoint* first = copyt.pMapTable.find(iter.first)->second;
        std::vector<GPFunctionTreePoint*> subtree;
        for (auto c : iter.second)
        {
            subtree.push_back(copyt.pMapTable.find(c)->second);
        }
        std::string name_;
        for (auto name_iter : names)
        {
            if (name_iter.second == iter.first)
            {
                name_ = name_iter.first;
                break;
            }
        }
        newp->addVariableSubTree(first, subtree, name_);
    }
    return newp;
}

GPFunctionTreePoint* GPFunctionTree::copy(const GPFunctionTreePoint* origin)
{
    GPFunctionTreePointCopy copyt(NULL);
    GPFunctionTreePoint* p = GPCONVERT(GPFunctionTreePoint, GPAbstractPoint::deepCopy((GPFunctionTreePoint*)origin, &copyt));
    return p;
}

void GPFunctionTreePoint::render(std::ostream& output) const
{
    switch (mType) {
        case FUNCTION:
        {
            auto mF = mData.pFunc;
            if (mF->shortname!="")
            {
                output <<mF->shortname;
            }
            else
            {
                output << mF->name;
            }
            output << "(";
            for (int i=0; i<(int)mChildren.size()-1; ++i)
            {
                auto pp = (GPFunctionTreePoint*)mChildren[i];
                pp->render(output);
                output << ", ";
            }
            if (mChildren.size()>0)
            {
                auto pp = (GPFunctionTreePoint*)mChildren[mChildren.size()-1];
                pp->render(output);
            }
            output << ")";
            break;
        }
        case INPUT:
            output << "x"<<mData.iInput;
            break;
        default:
            break;
    }
}


std::string GPFunctionTree::dump() const
{
    std::ostringstream output;
    mRoot->render(output);
    return output.str();
}
