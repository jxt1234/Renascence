/******************************************************************
   Copyright 2014, Jiang Xiao-tang

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
#include "GPSearchTree.h"

GPSearchTreePoint::GPSearchTreePoint(const GPFunctionDataBase* base, const std::vector<TYPEP>& output)
{
}

GPSearchTreePoint::~GPSearchTreePoint()
{
}

bool GPSearchTreePoint::vGrow()
{
    const PAIRP& p = mGroup[mCur];
    PAIRP forbid = _getDependFunction();
    for (int i=0; i<p.size(); ++i)
    {
    }
    return true;
}

const GPSearchTreePoint::PAIRP& GPSearchTreePoint::current()
{
    return mGroup[mCur];
}

GPSearchTreePoint::PAIRP GPSearchTreePoint::_getDependFunction()
{
    PAIRP res;
    GPSearchTreePoint* p = this->mDepend;
    while(NULL != p)
    {
        FUNC* f = NULL;
    }
}

bool GPSearchTreePoint::vNext()
{
    if (mCur >= mGroup.size())
    {
        return false;
    }
    mCur++;
    return true;
}
