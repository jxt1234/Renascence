#include "MGPThreadPool.h"
#include "MGPUtils.h"

class MGPThreadPool::ThreadWorker:public MGPThread
{
public:
    ThreadWorker(MGPThreadPool* pool, MGPSema* oksema, void* userData):mPool(pool), mReady(oksema), MGPThread(true)
    {
        mUserData = userData;
    }
    virtual ~ThreadWorker(){}
    
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
    MGPASSERT(threadNumber>1 && threadNumber < 100);
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
