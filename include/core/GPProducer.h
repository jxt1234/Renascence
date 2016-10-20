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
#include "lowlevelAPI/GPRefCount.h"
#include "lowlevelAPI/IGPFunction.h"
#include "IGPAutoDefFunction.h"
#include "GPFunctionTree.h"
#include <vector>
class GPFrontEndProducer;
class GPBackEndProducer;
class GPProducer:public IGPFunctionContext
{
public:
    GPProducer(GPFrontEndProducer* front, GPBackEndProducer* back, const GPFunctionDataBase* base);
    virtual ~GPProducer();
    
    IGPAutoDefFunction* createFunction(const std::vector<const IStatusType*>& outputs, const std::vector<const IStatusType*>& inputs) const;
    std::vector<GPPtr<IGPAutoDefFunction>> listAllFunction(const std::vector<const IStatusType*>& outputs, const std::vector<const IStatusType*>& inputs, int depth=0) const;
    IGPAutoDefFunction* createFunction(const std::string& formula, const std::vector<const IStatusType*>& inputs) const;
    IGPAutoDefFunction* createFunction(const GPTreeNode* node) const;
    
    std::vector<GPPtr<IGPAutoDefFunction>> listAllFunctionWithBackUp(const std::vector<const IStatusType*>& outputs, const std::vector<const IStatusType*>& inputs, int depth=0) const;
    
    inline const GPBackEndProducer* getBack() const {return mBack;}
    inline const GPFrontEndProducer* getFront() const {return mFront;}
    inline const GPFunctionDataBase* getDataBase() const {return mBase;}
    
    virtual IGPFunction* vCreateContentFunction(const std::string& formula, const std::string& parameters, const std::vector<const IStatusType*>& inputs) const override;
    virtual IKeyFunction* vCreateKeyFunction(const std::string& formula, const std::string& variable) const override;

private:
    GPBackEndProducer* mBack;
    GPFrontEndProducer* mFront;
    const GPFunctionDataBase* mBase;
};
#endif
