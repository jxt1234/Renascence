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
#include <string.h>
#include <vector>
#include "head.h"
#include "core/GPPieceFactory.h"

class GPPieceInMemory
{
public:
    GPPieceInMemory(const std::vector<unsigned int>& keydimesions):mKeyDimesions(keydimesions)
    {
        mMaxSize = 1;
        for (int i=0; i<keydimesions.size(); ++i)
        {
            GPASSERT(keydimesions[i]>0);
            mMaxSize *= keydimesions[i];
        }
        mPieces = new GPContents*[mMaxSize];
        ::memset(mPieces, 0, mMaxSize*sizeof(GPContents*));
    }
    ~GPPieceInMemory()
    {
        for (size_t i=0; i<mMaxSize; ++i)
        {
            if (NULL!=mPieces[i])
            {
                delete mPieces[i];
            }
        }
        delete [] mPieces;
    }
    
    GPContents* get(unsigned int* key, int keynum)
    {
        auto sum = _computePos(key, keynum);
        //GPASSERT(mPieces[sum]!=NULL);
        GPContents* res = mPieces[sum];
        mPieces[sum] = NULL;
        return res;
    }
    void set(GPContents* c, unsigned int* key, int keynum)
    {
        auto sum = _computePos(key, keynum);
        GPASSERT(mPieces[sum] == NULL);//TODO
        mPieces[sum] = c;
    }
    
private:
    size_t _computePos(unsigned int* key, int keynum)
    {
        GPASSERT(keynum == mKeyDimesions.size());
        size_t sum = 0;
        for (int i=0; i<keynum; ++i)
        {
            sum = sum*mKeyDimesions[i] + key[i];
        }
        GPASSERT(sum < mMaxSize);
        return sum;
    }
    GPContents** mPieces;
    std::vector<unsigned int> mKeyDimesions;
    size_t mMaxSize;
};

static void _freeMemoryPiece(void* meta)
{
    GPPieceInMemory* _meta = (GPPieceInMemory*)meta;
    delete _meta;
}

static GPContents* _load(void* meta, unsigned int* key, unsigned int keynum)
{
    GPPieceInMemory* _meta = (GPPieceInMemory*)meta;
    return _meta->get(key, keynum);
}

static void _save(void* meta, unsigned int* key, unsigned int keynum, GPContents* c)
{
    GPPieceInMemory* _meta = (GPPieceInMemory*)meta;
    _meta->set(c, key, keynum);
}

static void _createMemoryPieces(GPPieces& target, const std::vector<unsigned int>& keydimesions)
{
    GPASSERT(keydimesions.size() < 10);
    GPPieces* result = &target;
    ::memset(result, 0, sizeof(GPPieces));
    GPPieceInMemory* meta = new GPPieceInMemory(keydimesions);
    result->pMeta = (void*)meta;
    
    result->pFree = _freeMemoryPiece;
    result->pLoad = _load;
    result->pSave = _save;
    
    /*Set Up Keys*/
    result->nKeyNumber = (unsigned int)(keydimesions.size());
    for (int i=0; i<keydimesions.size(); ++i)
    {
        result->pKeySize[i] = keydimesions[i];
    }
}

GPPieces* GPPieceFactory::createMemoryPiece(const std::vector<unsigned int> &keydimesions)
{
    GPPieces* pieces = new GPPieces;
    _createMemoryPieces(*pieces, keydimesions);
    return pieces;
}
GPPieces* GPPieceFactory::createFilePiece(const std::vector<unsigned int> &keydimesions, const char* cacheDirpath, size_t maxMemoryCacheSize/*MB*/)
{
    return NULL;
}
