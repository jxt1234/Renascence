/******************************************************************
   Copyright 2013, Jiang Xiao-tang

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
#ifndef VECTOR_POINT_TREE_H
#define VECTOR_POINT_TREE_H
#include "math/carryTree.h"
#include "utils/debug.h"
class GPFunctionDataBase;
class computePoint:public carryPoint
{
friend class computeSearchTree;
public:
    computePoint(const std::vector<std::vector<int> >& data, const std::vector<int>& avail, const std::vector<const IStatusType*>& input, const GPFunctionDataBase* sys):mData(data), mAvail(avail), mCur(0), mInput(input), mSys(sys){}
    const std::vector<int>& getData() const{return mData[mAvail[mCur]];}
    std::vector<int> filter(const std::vector<std::vector<int> >& combo, const std::vector<int>& output);
protected:
    virtual bool vGrow();
    std::vector<int> getDependOutput();
    virtual bool vNext()
    {
        mCur++;
        bool res = true;
        if (mCur >=mAvail.size())
        {
            res = false;
            mCur = mAvail.size() - 1;
        }
        return res;
    }
private:
    const std::vector<const IStatusType*>& mInput;
    const std::vector<std::vector<int> >& mData;
    std::vector<int> mAvail;
    int mCur;
    int mParent;
    const GPFunctionDataBase* mSys;
};


class computeSearchTree:public carryTree<std::vector<int> >
{
public:
    computeSearchTree(computePoint* point){mRoot = point;}
    virtual std::vector<int> output();
    virtual bool readyToOutput();
};

#endif
