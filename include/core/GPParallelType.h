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
#ifndef CORE_GPPARALLELTYPE_H
#define CORE_GPPARALLELTYPE_H
#include <map>
#include <vector>
#include "utils/RefCount.h"
#include "GPFunctionDataBase.h"
#include "math/GPSingleTree.h"
class GPParallelType:public RefCount
{
public:
    struct Parallel
    {
        const GPFunctionDataBase::function* pFunc;
        GPPtr<GPSingleTree> pKeyFunc;
        std::map<int, std::vector<int>> mSplitInfo;
        std::map<int, GPPtr<GPSingleTree>> mOutputKey;
    };
};


#endif
