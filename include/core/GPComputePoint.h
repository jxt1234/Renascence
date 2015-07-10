/******************************************************************
   Copyright 2015, Jiang Xiao-tang

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
#ifndef CORE_GPCOMPUTEPOINT_H
#define CORE_GPCOMPUTEPOINT_H
#include "head.h"
#include "user/GPContents.h"
class GPComputePoint:public RefCount
{
public:
    GPComputePoint(computeFunction f, int n);
    GPComputePoint(computeFunction f, const std::vector<bool>& completeFlags);
    virtual ~GPComputePoint();
    bool vReceive(GPContents* inputs);
    GPContents* vCompute();
private:
    computeFunction mF;
    std::vector<bool> mFlags;
    GPContents mCache;
};
#endif
