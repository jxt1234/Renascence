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
#include "GPOptimizedKeyIterator.h"
#include "utils/GPDebug.h"
#include "utils/AutoStorage.h"
#include <set>
#include <string.h>

bool GPOptimizedKeyIterator::vNext(unsigned int* pInputKeys, unsigned int* pOutputKeys)
{
    bool res = mGroup->next(mCache, mCacheSize);
    if (!res)
    {
        return false;
    }
    pOutputKeys[0] = 0;
    for (int i=0; i<mMapPosForInput.size(); ++i)
    {
        pInputKeys[i] = mCache[mMapPosForInput[i]];
    }
    for (int i=0; i<mOutputPos.size(); ++i)
    {
        pOutputKeys[i] = pInputKeys[mOutputPos[i]];
    }
    return true;
}
std::pair<unsigned int, unsigned int> GPOptimizedKeyIterator::vGetSize() const
{
    if (mOutputPos.size() > 0)
    {
        return std::make_pair(mMapPosForInput.size(), mOutputPos.size());
    }
    return std::make_pair(mMapPosForInput.size(), 1);
}
bool GPOptimizedKeyIterator::vRewind(unsigned int* pInputKeys, unsigned int* pOutputKeys)
{
    mGroup->start(mCache, mCacheSize);
    auto size = this->vGetSize();
    ::memset(pInputKeys, 0, sizeof(unsigned int)*size.first);
    ::memset(pOutputKeys, 0, sizeof(unsigned int)*size.second);
    return true;
}

GPOptimizedKeyIterator::~GPOptimizedKeyIterator()
{
    delete mGroup;
    delete [] mCache;
}

GPOptimizedKeyIterator::GPOptimizedKeyIterator(GPPieces** inputs, int nInput, const GPParallelType::KEYS& outputKeys, const GPParallelType::KEYS& sameKeys)
{
    GPASSERT(sameKeys.size()>1);
    GPASSERT(NULL!=inputs && 0<nInput);

    /*Translate the sameKeys to input pos*/
    AUTOSTORAGE(sameKeysPos, unsigned int, (int)sameKeys.size());
    for (int k=0; k<sameKeys.size(); ++k)
    {
        int pos = sameKeys[k].second;
        for (int i=0; i<sameKeys[k].first; ++i)
        {
            pos+= inputs[i]->getKeySize().size();
        }
        sameKeysPos[k] = pos;
    }
    std::set<unsigned int> sameSet;
    /*Save first pos, the other pos deleted*/
    for (int i=0; i<sameKeys.size(); ++i)
    {
        sameSet.insert(sameKeysPos[i]);
    }
    
    /*Compute Total Input Dimesion*/
    unsigned int totalInputDimesion = 0;
    for (int i=0; i<nInput; ++i)
    {
        totalInputDimesion += inputs[i]->getKeySize().size();
    }
    
    //One pos for the same keys as 0, others in order
    mCacheSize = totalInputDimesion-(unsigned int)sameSet.size()+1;
    mCache = new unsigned int[mCacheSize];
    AUTOSTORAGE(dimesions, unsigned int, mCacheSize);
    dimesions[0] = 0;
    int pos = 1;
    int posOrigin = 0;
    for (int i=0; i<nInput; ++i)
    {
        auto piece = inputs[i];
        auto keysize = piece->getKeySize();
        for (int j=0; j<keysize.size(); ++j)
        {
            if (sameSet.find(posOrigin)==sameSet.end())
            {
                mMapPosForInput.push_back(pos);
                dimesions[pos] = keysize[j];
                pos = pos + 1;
            }
            else
            {
                mMapPosForInput.push_back(0);
                if (0 == dimesions[0] || dimesions[0] > keysize[j])
                {
                    dimesions[0] = keysize[j];
                }
            }
            posOrigin++;
        }
    }
//    printf("Dimesions:");
//    for (int i=0; i<mCacheSize; ++i)
//    {
//        printf("%d ", dimesions[i]);
//    }
//    printf("\n");
    mGroup = new GPCarryVaryGroup(dimesions, mCacheSize);
    for (int pos=0; pos < outputKeys.size(); ++pos)
    {
        auto p = outputKeys[pos];
        unsigned int outputPos = p.second;
        for (int i=0; i<p.first; ++i)
        {
            outputPos += inputs[i]->getKeySize().size();
        }
        mOutputPos.push_back(outputPos);
    }
}
