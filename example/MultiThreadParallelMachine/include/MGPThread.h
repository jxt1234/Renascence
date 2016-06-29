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
#ifndef UTILS_THREAD_H
#define UTILS_THREAD_H
#include <stdlib.h>
#include <thread>
#include <mutex>
#include "lowlevelAPI/GPRefCount.h"


class MGPSema:public GPRefCount
{
public:
    MGPSema();
    ~MGPSema();
    void wait();
    void post();
private:
    void* mData;
};

class MGPMutex
{
public:
    MGPMutex(){}
    ~MGPMutex(){}
    void lock()
    {
        mData.lock();
    }
    void unlock()
    {
        mData.unlock();
    }
private:
    std::mutex mData;
};

class MGPAutoMutex
{
public:
    MGPAutoMutex(MGPMutex& _m):m(_m){m.lock();}
    ~MGPAutoMutex(){m.unlock();}
private:
    MGPMutex& m;
};


class MGPThread
{
public:
    MGPThread(bool loop = false);
    virtual ~MGPThread();
    bool start();
    bool stop();
    bool wake();
    
    virtual bool readyToRun() = 0;
    virtual void run() = 0;
private:
    static void* threadLoop(void* t);
    bool platform_create();
    bool platform_destroy();
    bool platform_wake();
    void platform_wait();
    bool platform_join();
    void* mData;
    bool mLoop;
};


#endif
