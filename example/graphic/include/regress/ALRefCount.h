#ifndef INCLUDE_UTILS_ALREFCOUNT_H
#define INCLUDE_UTILS_ALREFCOUNT_H
class ALRefCount
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
        ALRefCount():mNum(1){}
        ALRefCount(const ALRefCount& f):mNum(f.mNum){}
        void operator=(const ALRefCount& f)
        {
            if (this != &f)
            {
                mNum = f.mNum;
            }
        }
        virtual ~ALRefCount(){}
    private:
        int mNum;
};

#define ALSAVEUNREF(x)\
    if (NULL!=(x)) (x)->decRef();
#endif
