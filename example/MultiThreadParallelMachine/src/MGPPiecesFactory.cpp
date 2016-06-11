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
#include "MGPPiecesFactory.h"
#include "MGPUtils.h"
#include "MGPThread.h"

class MGPPiecesLockMemory : public GPPieces
{
public:
    MGPPiecesLockMemory(const std::vector<unsigned int>& keydimesions):mKeyDimesions(keydimesions)
    {
        mMaxSize = 1;
        if (keydimesions.empty())
        {
            nKeyNumber = 1;
            pKeySize[0] = 1;
        }
        else
        {
            nKeyNumber = (unsigned int)keydimesions.size();
            for (int i=0; i<keydimesions.size(); ++i)
            {
                MGPASSERT(keydimesions[i]>0);
                mMaxSize *= keydimesions[i];
                pKeySize[i] = keydimesions[i];
            }
        }
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
        }
        delete [] mPieces;
    }
    
    virtual GPContents* vLoad(unsigned int* pKey, unsigned int keynum)
    {
        auto sum = _computePos(pKey, keynum);
        GPContents* res = mPieces[sum];
        MGPASSERT(NULL!=res);
        res->addRef();
        return res;
    }
    
    virtual void vSave(unsigned int* pKey, unsigned int keynum, GPContents* c)
    {
        auto sum = _computePos(pKey, keynum);
        c->addRef();
        if (NULL != mPieces[sum])
        {
            mPieces[sum]->decRef();
        }
        mPieces[sum] = c;
    }

private:
    size_t _computePos(unsigned int* key, int keynum)
    {
        MGPASSERT(keynum >= mKeyDimesions.size());
        size_t sum = 0;
        for (int i=0; i<mKeyDimesions.size(); ++i)
        {
            sum = sum*mKeyDimesions[i] + key[i];
        }
        MGPASSERT(sum < mMaxSize);
        return sum;
    }
    GPContents** mPieces;
    std::vector<unsigned int> mKeyDimesions;
    size_t mMaxSize;
};





GPPieces* MGPPiecesFactory::create(const std::vector<unsigned int>& keydimesions)
{
    return new MGPPiecesLockMemory(keydimesions);
}
