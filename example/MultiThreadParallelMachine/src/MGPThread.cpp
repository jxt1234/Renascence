#include "MGPThread.h"
#include "MGPUtils.h"

MGPThread::MGPThread(bool loop):mData(NULL)
{
    mLoop = loop;
}
MGPThread::~MGPThread()
{
    if (NULL != mData)
    {
        bool join_result = stop();
        MGPASSERT(join_result);
    }
}

bool MGPThread::start()
{
    return platform_create();
}

bool MGPThread::stop()
{
    mLoop = false;
    wake();
    return platform_destroy();
}

bool MGPThread::wake()
{
    return platform_wake();
}

void* MGPThread::threadLoop(void* t)
{
    MGPASSERT(NULL!=t);
    MGPThread* th = (MGPThread*)t;
    bool res = th->readyToRun();
    MGPASSERT(res);
    do
    {
        th->run();
        if (th->mLoop)
        {
            th->platform_wait();
        }
    }while(th->mLoop);
    return NULL;
}

