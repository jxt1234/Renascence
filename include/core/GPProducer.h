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
#ifndef CORE_GPPRODUCER_H
#define CORE_GPPRODUCER_H
#include "utils/RefCount.h"
#include "IGPAutoDefFunction.h"
#include "GPFunctionTree.h"
#include <vector>
class GPFrontEndProducer;
class GPBackEndProducer;
class GPProducer:public RefCount
{
public:
    GPProducer(GPFrontEndProducer* front, GPBackEndProducer* back, const GPFunctionDataBase* base);
    virtual ~GPProducer();
    
    IGPAutoDefFunction* createFunction(const std::vector<const IStatusType*>& outputs, const std::vector<const IStatusType*>& inputs);
    std::vector<GPPtr<IGPAutoDefFunction>> listAllFunction(const std::vector<const IStatusType*>& outputs, const std::vector<const IStatusType*>& inputs);
    IGPAutoDefFunction* createFunction(const std::string& formula, const std::vector<const IStatusType*>& inputs);
    IGPAutoDefFunction* createFunction(const GPTreeNode* node);
    
    std::vector<GPPtr<IGPAutoDefFunction>> listAllFunctionWithBackUp(const std::vector<const IStatusType*>& outputs, const std::vector<const IStatusType*>& inputs);
    
    inline const GPBackEndProducer* getBack() const {return mBack;}
    inline const GPFrontEndProducer* getFront() const {return mFront;}
private:
    GPBackEndProducer* mBack;
    GPFrontEndProducer* mFront;
    const GPFunctionDataBase* mBase;
};
#endif
