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
#ifndef MGPTHREADPOOL_H
#define MGPTHREADPOOL_H
#include "lowlevelAPI/GPRefCount.h"
#include "MGPThread.h"
#include <queue>
#include <map>
class MGPThreadPool:public GPRefCount
{
public:
    MGPThreadPool(int threadNumber);
    void start();
    virtual ~MGPThreadPool();
    
    class Runnable:public GPRefCount
    {
    public:
        Runnable(){}
        virtual ~Runnable(){}
        virtual void vRun() = 0;
    };
    
    MGPSema* pushTask(GPPtr<Runnable> runnables);
    GPPtr<Runnable> queueTask();
    void completeTask(const Runnable* runnable);
    
private:
    class ThreadWorker;
    friend class ThreadWorker;
    MGPMutex mQueueMutex;
    std::queue<GPPtr<Runnable>> mQueues;
    std::map<const Runnable*, MGPSema*> mSemaMap;
    
    std::vector<MGPThread*> mThreads;
};
#endif
