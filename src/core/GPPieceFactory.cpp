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
    GPPieceInMemory(const std::vector<unsigned int>& keydimesions):GPPieces(keydimesions)
    {
        mKeySize = keydimesions;
        mMaxSize = 1;
        for (int i=0; i<keydimesions.size(); ++i)
        {
            GPASSERT(keydimesions[i]>0);
            mMaxSize *= keydimesions[i];
        }
        mPieces.resize(mMaxSize);
    }
    virtual ~GPPieceInMemory()
    {
    }
    
    virtual GPContents::CONTENT vLoad(unsigned int* pKey, unsigned int keynum) override
    {
        auto sum = _computePos(pKey, keynum);
        return mPieces[sum];
    }
    
    virtual void vSave(unsigned int* pKey, unsigned int keynum, GPContents::CONTENT c) override
    {
        auto sum = _computePos(pKey, keynum);
        mPieces[sum] = c;
    }
    

private:
    size_t _computePos(unsigned int* key, int keynum)
    {
        GPASSERT(keynum <= mKeySize.size() || (keynum==1 && mKeySize.size()==0));
        size_t sum = 0;
        for (int i=0; i<keynum; ++i)
        {
            sum = sum*mKeySize[i] + key[i];
        }
        GPASSERT(sum < mMaxSize);
        return sum;
    }
    std::vector<GPContents::CONTENT> mPieces;
    size_t mMaxSize;
    std::vector<unsigned int> mKeySize;
};

class GPLocalFilePiece : public GPPieces
{
public:
    GPLocalFilePiece(const std::vector<unsigned int> &keydimesions, const char* path, const IStatusType* type):GPPieces(keydimesions)
    {
        GPASSERT(NULL!=type);
        GPASSERT(NULL!=path);
        mType = type;
        mPath = path;
    }
    virtual ~GPLocalFilePiece()
    {
    }
    
    std::string generatePath(unsigned int* pKey, unsigned int keynum, const std::string& type)
    {
        return GPPieceFactory::getFilePath(pKey, keynum, type, mPath);
    }

    virtual GPContents::CONTENT vLoad(unsigned int* pKey, unsigned int keynum) override
    {
        GPASSERT(keynum>0);
        std::string path = generatePath(pKey, keynum, mType->name());
        GPPtr<GPStream> readStream = GPStreamFactory::NewStream(path.c_str());
        GPASSERT(NULL!=readStream.get());
        return new GPContents::GP_Unit(mType->vLoad(readStream.get()), mType);
    }
    
    virtual void vSave(unsigned int* pKey, unsigned int keynum, GPContents::CONTENT c) override
    {
        GPASSERT(NULL!=c.get());
        GPASSERT(keynum>0);
        auto type = c->type();
        std::string path = generatePath(pKey, keynum, type->name());
        GPPtr<GPWStream> writeStream = GPStreamFactory::NewWStream(path.c_str());
        type->vSave(c->content(), writeStream.get());
    }

private:
    std::string mPath;
    const IStatusType* mType;
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

GPPieces* GPPieceFactory::createLocalFilePiece(const std::vector<unsigned int> &keydimesions, const IStatusType* types, const char* srcPath, size_t maxMemoryCacheSize/*MB*/)
{
    return new GPLocalFilePiece(keydimesions, srcPath, types);
}

std::string GPPieceFactory::getFilePath(unsigned int* pKey, unsigned int keynum, const std::string& type, const std::string& basicPath)
{
    std::ostringstream output;
    output << basicPath << "/"<<type;
    for (int i=0; i<keynum; ++i)
    {
        output << "_" << pKey[i];
    }
    return output.str();
}
