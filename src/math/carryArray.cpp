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
#include "math/carryArray.h"
carryArray::carryArray(int size, int maxNumber)
{
    mQueue.clear();
    for (int i=0; i<size; ++i)
    {
        mQueue.push_back(0);
    }
    mCarryNumber = maxNumber;
}

bool carryArray::reachEnd()
{
    if (!mQueue.empty())
    {
        if (mQueue[0]<mCarryNumber)
        {
            return false;
        }
    }
    return true;
}
carryArray& carryArray::operator++()
{
    int cur = mQueue.size()-1;
    mQueue[cur]++;
    runCarry();
    return *this;
}
void carryArray::runCarry()
{
    for (int cur = mQueue.size()-1; cur > 0; --cur)
    {
        if (mQueue[cur]>=mCarryNumber)
        {
            mQueue[cur-1]+=mQueue[cur]/mCarryNumber;
            mQueue[cur] = mQueue[cur]%mCarryNumber;
        }
    }
}
