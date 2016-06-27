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
#include "MGPThreadPool.h"
#include "MGPUtils.h"
#include <string.h>

class MGPThreadPool::ThreadWorker:public MGPThread
{
public:
    ThreadWorker(MGPThreadPool* pool, MGPSema* oksema, void* userData):mPool(pool), mReady(oksema), MGPThread(true)
    {
        mUserData = userData;
    }
    virtual ~ThreadWorker()
    {
        
    }
    
    void setUserData(void* data)
    {
        mUserData = data;
    }
    
    virtual void run() override
    {
        while (true)
        {
            GPPtr<Runnable> runable = mPool->queueTask();
            if (NULL == runable.get())
            {
                break;
            }
            runable->vRun(mUserData);
            mPool->completeTask(runable.get());
        }
    }
    
    virtual bool readyToRun() override
    {
        mReady->post();
        return true;
    }

private:
    MGPThreadPool* mPool;
    MGPSema* mReady;
    void* mUserData;
};

MGPThreadPool::MGPThreadPool(std::vector<void*> userdata)
{
    auto threadNumber = userdata.size();
    mThreadNumber = threadNumber;
    //TODO
    MGPASSERT(threadNumber>=1 && threadNumber < 100);
    std::vector<MGPSema*> threadOpenSeam;
    for (int i=0; i<threadNumber; ++i)
    {
        MGPSema* threadSema = new MGPSema;
        threadOpenSeam.push_back(threadSema);
        mThreads.push_back(new ThreadWorker(this, threadSema, userdata[i]));
    }
    for (int i=0; i<threadNumber; ++i)
    {
        mThreads[i]->start();
    }
    for (auto s : threadOpenSeam)
    {
        s->wait();
        delete s;
    }
}


MGPThreadPool::~MGPThreadPool()
{
    for (auto t : mThreads)
    {
        delete t;
    }
}

void MGPThreadPool::completeTask(const Runnable* runnable)
{
    MGPAutoMutex __l(mQueueMutex);
    auto iter = mSemaMap.find(runnable);
    if (iter!=mSemaMap.end())
    {
        iter->second->post();
        mSemaMap.erase(runnable);
    }
}


MGPSema* MGPThreadPool::pushTask(Runnable* runnable)
{
    MGPASSERT(NULL!=runnable);
    MGPAutoMutex __l(mQueueMutex);
    MGPSema* sema = new MGPSema;
    const Runnable* run = runnable;
    mQueues.push(runnable);
    mSemaMap.insert(std::make_pair(run, sema));
    for (auto t : mThreads)
    {
        t->wake();
    }
    return sema;
}
MGPThreadPool::Runnable* MGPThreadPool::queueTask()
{
    MGPAutoMutex __l(mQueueMutex);
    if (mQueues.empty())
    {
        return NULL;
    }
    Runnable* res = mQueues.front();
    mQueues.pop();
    return res;
}


class MergeRunnable:public MGPThreadPool::Runnable
{
public:
    MergeRunnable(const std::vector<Runnable*>& runnable)
    {
        mRunnables = runnable;
    }
    virtual ~MergeRunnable()
    {
        for (auto r : mRunnables)
        {
            r->decRef();
        }
    }
    virtual void vRun(void* data)
    {
        for (auto& r : mRunnables)
        {
            r->vRun(data);
        }
    }
private:
    std::vector<Runnable*> mRunnables;
};


MGPThreadPool::Runnable* MGPThreadPool::mergeRunnables(const std::vector<Runnable*>& runnable)
{
    return new MergeRunnable(runnable);
}
