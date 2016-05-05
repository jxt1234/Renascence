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
    platform_join();
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

