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
#include "computeSystem.h"
#include "utils/debug.h"
class computePoint:public carryPoint
{
friend class computeSearchTree;
public:
    computePoint(const vector<vector<int> >& data, const vector<int>& avail, computeSystem* sys):mData(data), mAvail(avail), mCur(0), mSys(sys){}
    const vector<int>& getData() const{return mData[mAvail[mCur]];}
    vector<int> filter(const vector<vector<int> >& combo, const vector<int>& output);
protected:
    virtual bool vGrow();
    vector<int> getDependOutput();
    virtual bool vNext(){mCur++;return mCur<mAvail.size();}
private:
    computeSystem* mSys;
    const vector<vector<int> >& mData;
    vector<int> mAvail;
    int mCur;
    int mParent;
};


class computeSearchTree:public carryTree<vector<int> >
{
public:
    computeSearchTree(computePoint* point){mRoot = point;}
    virtual vector<int> output();
    virtual bool readyToOutput();
};

#endif
