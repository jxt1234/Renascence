#include "MGPPiecesFactory.h"
#include "MGPUtils.h"
#include "MGPThread.h"

class MGPPiecesLockMemory : public GPPieces
{
public:
    MGPPiecesLockMemory(const std::vector<unsigned int>& keydimesions):mKeyDimesions(keydimesions)
    {
        mMaxSize = 1;
        nKeyNumber = (unsigned int)keydimesions.size();
        for (int i=0; i<keydimesions.size(); ++i)
        {
            MGPASSERT(keydimesions[i]>0);
            mMaxSize *= keydimesions[i];
            pKeySize[i] = keydimesions[i];
        }
        mMutexes = new MGPMutex[mMaxSize];
        mPieces = new GPContents*[mMaxSize];
        ::memset(mPieces, 0, mMaxSize*sizeof(GPContents*));
    }
    virtual ~MGPPiecesLockMemory()
    {
        for (size_t i=0; i<mMaxSize; ++i)
        {
            if (NULL!=mPieces[i])
            {
                mPieces[i]->decRef();
            }
            mMutexes[i].unlock();
        }
        delete [] mPieces;
        delete [] mMutexes;
    }
    
    virtual GPContents* vLoad(unsigned int* pKey, unsigned int keynum)
    {
        auto sum = _computePos(pKey, keynum);
        MGPAutoMutex __lock(mMutexes[sum]);
        GPContents* res = mPieces[sum];
        if (NULL != res)
        {
            res->addRef();
        }
        return res;
    }
    
    virtual void vSave(unsigned int* pKey, unsigned int keynum, GPContents* c)
    {
        auto sum = _computePos(pKey, keynum);
        MGPAutoMutex __lock(mMutexes[sum]);
        if (NULL != mPieces[sum])
        {
            mPieces[sum]->decRef();
        }
        mPieces[sum] = c;
        c->addRef();
    }
    
private:
    size_t _computePos(unsigned int* key, int keynum)
    {
        MGPASSERT(keynum == mKeyDimesions.size());
        size_t sum = 0;
        for (int i=0; i<keynum; ++i)
        {
            sum = sum*mKeyDimesions[i] + key[i];
        }
        MGPASSERT(sum < mMaxSize);
        return sum;
    }
    GPContents** mPieces;
    MGPMutex* mMutexes;
    std::vector<unsigned int> mKeyDimesions;
    size_t mMaxSize;
};





GPPieces* MGPPiecesFactory::create(const std::vector<unsigned int>& keydimesions)
{
    return new MGPPiecesLockMemory(keydimesions);
}
