#ifndef UTILS_THREAD_H
#define UTILS_THREAD_H
#include <stdlib.h>
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
    MGPMutex();
    ~MGPMutex();
    void lock();
    void unlock();
private:
    void* mData;
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
