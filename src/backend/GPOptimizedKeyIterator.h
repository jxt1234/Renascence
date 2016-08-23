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
#ifndef BACKEND_GPOPTIMIZEDKEYITERATOR_H
#define BACKEND_GPOPTIMIZEDKEYITERATOR_H
#include <vector>
#include "backend/IGPKeyIterator.h"
#include "math/GPCarryVaryGroup.h"
#include "lowlevelAPI/GPPieces.h"
#include "lowlevelAPI/IGPFunction.h"
#include "lowlevelAPI/GPParallelType.h"

class GPOptimizedKeyIterator:public IGPKeyIterator
{
public:
    GPOptimizedKeyIterator(GPPieces** inputs, int nInput, const GPParallelType::KEYS& outputKeys, const GPParallelType::KEYS& sameKeys);
    virtual ~GPOptimizedKeyIterator();

    virtual bool vNext(unsigned int* pInputKeys, unsigned int* pOutputKeys) override;
    virtual std::pair<unsigned int, unsigned int> vGetSize() const override;
    virtual bool vRewind(unsigned int* pInputKeys, unsigned int* pOutputKeys) override;
private:
    std::vector<unsigned int> mOutputPos;
    GPCarryVaryGroup* mGroup;

    unsigned int* mCache;
    unsigned int mCacheSize;

    std::vector<unsigned int> mMapPosForInput;
};

#endif
