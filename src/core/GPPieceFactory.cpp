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
#include <sstream>
#include "head.h"
#include "core/GPPieceFactory.h"
#include "core/GPStreamFactory.h"
#include "platform/GPSystem.h"

class GPPieceInMemory : public GPPieces
{
public:
    GPPieceInMemory(const std::vector<unsigned int>& keydimesions):mKeyDimesions(keydimesions)
    {
        mMaxSize = 1;
        if (keydimesions.empty())
        {
            pKeySize[0] = 1;
            nKeyNumber = 1;
        }
        else
        {
            nKeyNumber = (unsigned int)keydimesions.size();
            for (int i=0; i<keydimesions.size(); ++i)
            {
                GPASSERT(keydimesions[i]>0);
                mMaxSize *= keydimesions[i];
                pKeySize[i] = keydimesions[i];
            }
        }
        mPieces = new GPContents*[mMaxSize];
        ::memset(mPieces, 0, mMaxSize*sizeof(GPContents*));
    }
    virtual ~GPPieceInMemory()
    {
        //FUNC_PRINT_ALL(this, p);
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
        if (NULL == res)
        {
            return NULL;
        }
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
        GPASSERT(keynum == mKeyDimesions.size() || (keynum==1 && mKeyDimesions.size()==0));
        size_t sum = 0;
        for (int i=0; i<mKeyDimesions.size(); ++i)
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

class GPPieceSimpleOutputFile : public GPPieces
{
public:
    GPPieceSimpleOutputFile(const char* path, const char* postfix, std::vector<unsigned int> keys)
    {
        system_make_dir(path);
        for (int i=0; i<keys.size(); ++i)
        {
            std::ostringstream os;
            os << path << "/" << keys[i]<<"."<<postfix;
            mFiles.push_back(os.str());
        }
    }
    virtual ~GPPieceSimpleOutputFile()
    {
    }
    
    virtual GPContents* vLoad(unsigned int* pKey, unsigned int keynum)
    {
        GPASSERT(false);
        return NULL;
    }
    
    virtual void vSave(unsigned int* pKey, unsigned int keynum, GPContents* c)
    {
        GPASSERT(1 == keynum);
        GPASSERT(NULL!=pKey);
        GPASSERT(*pKey < mFiles.size());
        GPWStream* stream = GPStreamFactory::NewWStream(mFiles[*pKey].c_str());
        delete stream;
    }
    
private:
    std::vector<std::string> mFiles;
};



static GPPieces* _createMemoryPieces(const std::vector<unsigned int>& keydimesions)
{
    GPASSERT(keydimesions.size() < 10);
    return new GPPieceInMemory(keydimesions);
}

GPPieces* GPPieceFactory::createMemoryPiece(const std::vector<unsigned int> &keydimesions)
{
    return _createMemoryPieces(keydimesions);
}

GPPieces* GPPieceFactory::createInputFilePiece(const IStatusType* type, const char* srcPath, size_t maxMemoryCacheSize)
{
    auto files = system_list_dir_files(srcPath);
    GPASSERT(!files.empty());
    GPPieces* pieces = new GPPieceInMemory(std::vector<unsigned int>{(unsigned int)files.size()});
    for (int i=0; i<files.size(); ++i)
    {
        unsigned int key = i;
        GPStream* stream = GPStreamFactory::NewStream(files[i].c_str());
        GPContents* content = new GPContents;
        auto c = type->vLoad(stream);
        delete stream;
        content->push(c, type);
        pieces->vSave(&key, 1, content);
        content->decRef();
    }
    return pieces;
}

GPPieces* GPPieceFactory::createOutputFilePiece(const std::vector<unsigned int> &keydimesions, const char* dstPath, const char* postfix)
{
    return new GPPieceSimpleOutputFile(dstPath, postfix, keydimesions);
}
