#include "utils/statusMachine.h"

using namespace std;

void statusMachine::run()
{
    if (0>=mStatus && mStatus<mFuncs.size())
    {
        mStatus = mFuncs[mStatus](this);
    }
}
