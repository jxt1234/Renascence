#ifndef INCLUDE_LOWLEVELAPI_GPREFCOUNT_H
#define INCLUDE_LOWLEVELAPI_GPREFCOUNT_H
#include <stdlib.h>
class GPRefCount
{
public:
    void addRef()
    {
        mNum++;
    }
    void decRef()
    {
        --mNum;
        if (0 >= mNum)
        {
            delete this;
        }
    }
protected:
    inline int count() const{return mNum;}
    GPRefCount():mNum(1){}
    GPRefCount(const GPRefCount& f):mNum(f.mNum){}
    void operator=(const GPRefCount& f)
    {
        if (this != &f)
        {
            mNum = f.mNum;
        }
    }
    virtual ~GPRefCount(){}
private:
    int mNum;
};
#define GP_SAFE_UNREF(x)\
if (NULL!=(x)) {(x)->decRef();}
#define GP_SAFE_REF(x)\
if (NULL!=(x)) (x)->addRef();

#define GP_SAFE_ASSIGN(dst, src) \
{\
if (src!=NULL)\
{\
src->addRef();\
}\
if (dst!=NULL)\
{\
dst->decRef();\
}\
dst = src;\
}

template <typename T>
class GPPtr {
public:
    GPPtr() : mT(NULL) {}
    GPPtr(T* obj) : mT(obj) {}
    GPPtr(const GPPtr& o) : mT(o.mT) { GP_SAFE_REF(mT); }
    ~GPPtr() { GP_SAFE_UNREF(mT); }
    
    GPPtr& operator=(const GPPtr& rp) {
        GP_SAFE_ASSIGN(mT, rp.mT);
        return *this;
    }
    GPPtr& operator=(T* obj) {
        GP_SAFE_UNREF(mT);
        mT = obj;
        return *this;
    }
    
    T* get() const { return mT; }
    T& operator*() const { return *mT; }
    T* operator->() const { return mT; }
    
private:
    T* mT;
};


#endif
