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
#include "GPBasicKeyIterator.h"
#include "utils/GPDebug.h"
#include "utils/AutoStorage.h"
#include <string.h>
bool GPBasicKeyIterator::vNext(unsigned int* pInputKeys, unsigned int* pOutputKeys)
{
    unsigned int conditionRes = -1;
    do
    {
        bool res = mGroup->next(mCache, mCacheSize);
        if (!res)
        {
            return false;
        }
        if (NULL == mCondition)
        {
            break;
        }
        conditionRes = mCondition->vRun(mCache, mCacheSize);
    } while (conditionRes <= 0);
    ::memcpy(pInputKeys, mCache, mCacheSize*sizeof(unsigned int));
    pOutputKeys[0] = 0;
    for (int i=0; i<mOutputPos.size(); ++i)
    {
        pOutputKeys[i] = pInputKeys[mOutputPos[i]];
    }
    return true;
}

GPBasicKeyIterator::GPBasicKeyIterator(GPPieces** inputs, int inputNumber, const GPParallelType::KEYS& outputKeys, IKeyFunction* condition)
{
    mCondition = condition;
    GPASSERT(NULL!=inputs);
    GPASSERT(inputNumber>0);
    /*Compute all dimesions*/
    unsigned int sumDim = 0;
    for (int i=0; i<inputNumber; ++i)
    {
        sumDim += inputs[i]->getKeySize().size();
    }
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
    mCacheSize = sumDim;
    mCache = new unsigned int[sumDim];
    AUTOSTORAGE(keyDimesions, unsigned int, sumDim);
    GPASSERT(NULL!=mCache && NULL!=keyDimesions);
    unsigned int pos = 0;
    for (int i=0; i<inputNumber; ++i)
    {
        auto keysize = inputs[i]->getKeySize();
        ::memcpy(keyDimesions+pos, keysize.data(), sizeof(unsigned int)*keysize.size());
        pos += keysize.size();
    }
    mGroup = new GPCarryVaryGroup(keyDimesions, sumDim);
    mGroup->start(mCache, mCacheSize);
}

GPBasicKeyIterator::~GPBasicKeyIterator()
{
    delete mGroup;
    delete [] mCache;
}

std::pair<unsigned int, unsigned int> GPBasicKeyIterator::vGetSize() const
{
    if (mOutputPos.empty())
    {
        return std::make_pair(mCacheSize, 1);
    }
    return std::make_pair(mCacheSize, mOutputPos.size());
}

bool GPBasicKeyIterator::vRewind(unsigned int* pInputKeys, unsigned int* pOutputKeys)
{
    mGroup->start(mCache, mCacheSize);
    unsigned int conditionRes = -1;
    do
    {
        if (NULL == mCondition)
        {
            break;
        }
        conditionRes = mCondition->vRun(mCache, mCacheSize);
        if (conditionRes > 0)
        {
            break;
        }
        bool res = mGroup->next(mCache, mCacheSize);
        if (!res)
        {
            return false;
        }
    } while (true);
    ::memcpy(pInputKeys, mCache, mCacheSize*sizeof(unsigned int));
    pOutputKeys[0] = 0;
    for (int i=0; i<mOutputPos.size(); ++i)
    {
        pOutputKeys[i] = pInputKeys[mOutputPos[i]];
    }
    return true;
}

