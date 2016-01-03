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
#ifndef FRONTEND_GPFUNCTIONFRONTENDPRODUCER_H
#define FRONTEND_GPFUNCTIONFRONTENDPRODUCER_H
#include "GPFrontEndProducer.h"
#include "GPProducerUtils.h"
class GPFunctionFrontEndProducer:public GPFrontEndProducer
{
public:
    virtual GPFunctionTree* vCreateOneFunction(const std::vector<const IStatusType*>& outputType, const std::vector<const IStatusType*>& inputType) const override;
    virtual std::vector<GPFunctionTree*> vCreateAllFunction(const std::vector<const IStatusType*>& outputType, const std::vector<const IStatusType*>& inputType) const override;
    virtual GPFunctionTree* vCreateFromFormula(const std::string& formula, const std::vector<const IStatusType*> inputTypes) const override;
    virtual int vMapStructure(GPFunctionTree* tree, GPParameter* para, int depth, bool& changed) const override;

    GPFunctionFrontEndProducer(const GPFunctionDataBase* base);
    virtual ~GPFunctionFrontEndProducer();
private:
    GPProducerUtils mUtils;
};

#endif
