#ifndef INCLUDE_UTILS_REFCOUNT_H
#define INCLUDE_UTILS_REFCOUNT_H
#include <stdlib.h>
class RefCount
{
    public:
        inline int count() const{return mNum;}
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
        RefCount():mNum(1){}
        RefCount(const RefCount& f):mNum(f.mNum){}
        void operator=(const RefCount& f)
        {
            if (this != &f)
            {
                mNum = f.mNum;
            }
        }
        virtual ~RefCount(){}
    private:
        int mNum;
};
#define SAFE_UNREF(x)\
    if (NULL!=(x)) {(x)->decRef();}
#define SAFE_REF(x)\
    if (NULL!=(x)) (x)->addRef();

#define SAFE_ASSIGN(dst, src) \
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
        GPPtr(const GPPtr& o) : mT(o.mT) { SAFE_REF(mT); }
        ~GPPtr() { SAFE_UNREF(mT); }

        GPPtr& operator=(const GPPtr& rp) {
            SAFE_ASSIGN(mT, rp.mT);
            return *this;
        }
        GPPtr& operator=(T* obj) {
            SAFE_UNREF(mT);
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
