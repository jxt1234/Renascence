#include "MGPExecutor.h"

MGPExecutor::MGPExecutor(int threadNum)
{
    mPool = new MGPThreadPool(threadNum);
}

MGPExecutor::~MGPExecutor()
{
    delete mPool;
}


bool MGPExecutor::vRun(GPPieces* output, GPPieces** inputs, int inputNumber) const
{
    return true;
}
