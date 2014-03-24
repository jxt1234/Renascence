#ifndef INCLUDE_UTILS_REFCOUNT_H
#define INCLUDE_UTILS_REFCOUNT_H
#include <iostream>
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
#endif
