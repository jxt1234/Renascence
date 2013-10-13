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
