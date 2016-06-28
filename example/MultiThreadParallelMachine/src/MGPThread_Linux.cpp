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
#include <pthread.h>
#include <assert.h>
#include <stdio.h>

struct TData
{
    pthread_t id;
    MGPSema* lock;
};

bool MGPThread::platform_create()
{
    TData* data = new TData;
    data->lock = NULL;
    if (mLoop)
    {
        data->lock = new MGPSema;
    }
    mData = (void*)data;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    //pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

//    pthread_attr_setschedpolicy(&attr, SCHED_RR);
//    struct sched_param param;
//    int rs = pthread_attr_getschedparam(&attr, &param);
//    assert(rs == 0);
//    param.sched_priority = 99;
//    pthread_attr_setschedparam(&attr, &param);

    pthread_create(&(data->id), &attr, MGPThread::threadLoop, this);
    pthread_attr_destroy(&attr);

//    pthread_create(&(data->id), NULL, MGPThread::threadLoop, this);

    return true;
}

bool MGPThread::platform_wake()
{
    assert(NULL!=mData);
    TData* data = (TData*)mData;
    data->lock->post();
    return true;
}

bool MGPThread::platform_destroy()
{
    assert(NULL!=mData);
    TData* data = (TData*)mData;
    if (NULL != data->lock)
    {
        delete data->lock;
    }
    delete data;
    mData = NULL;
    return true;
}

bool MGPThread::platform_join()
{
    assert(NULL!=mData);
    TData* data = (TData*)mData;
    pthread_join(data->id, NULL);
    return true;
}

void MGPThread::platform_wait()
{
    assert(NULL!=mData);
    TData* data = (TData*)mData;
    data->lock->wait();
}

class SemaData
{
public:
    SemaData()
    {
        pthread_mutex_init(&mMutex, NULL);
        pthread_cond_init(&mCondition, NULL);
        mNum = 0;
    }
    ~SemaData()
    {
        pthread_cond_destroy(&mCondition);
        pthread_mutex_destroy(&mMutex);
    }
    
    void wait()
    {
        pthread_mutex_lock(&mMutex);
        mNum--;
        while (0 > mNum)
        {
            pthread_cond_wait(&mCondition, &mMutex);
        }
        pthread_mutex_unlock(&mMutex);
    }

    void post()
    {
        pthread_mutex_lock(&mMutex);
        mNum++;
        pthread_mutex_unlock(&mMutex);
        
        pthread_cond_signal(&mCondition);
    }
private:
    pthread_mutex_t mMutex;
    pthread_cond_t mCondition;
    int mNum;
};

MGPSema::MGPSema()
{
    SemaData* s = new SemaData;
    mData = (void*)s;
}

MGPSema::~MGPSema()
{
    assert(NULL!=mData);
    SemaData* s = (SemaData*)(mData);
    delete s;
}

void MGPSema::wait()
{
    assert(NULL!=mData);
    SemaData* s = (SemaData*)(mData);
    s->wait();
}

void MGPSema::post()
{
    assert(NULL!=mData);
    SemaData* s = (SemaData*)(mData);
    s->post();
}
