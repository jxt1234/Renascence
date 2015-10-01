/******************************************************************
 Copyright 2013, Jiang Xiao-tang
 
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
#ifndef GENERATE_SYSTEM_H
#define GENERATE_SYSTEM_H

#include "core/GPFunctionDataBase.h"
#include "core/GPProducer.h"
#include "GPTreeADF.h"
#include "GPProducerUtils.h"

class GPTreeProducer:public GPProducer
{
public:
    virtual IGPAutoDefFunction* vCreateFunction(const std::vector<const IStatusType*>& outputType, const std::vector<const IStatusType*>& inputType, bool inputRepeat = true, bool random = false) const;
    virtual std::vector<IGPAutoDefFunction*> vCreateAllFunction(const std::vector<const IStatusType*>& outputType, const std::vector<const IStatusType*>& inputType, bool inputRepeat = true) const;
    virtual IGPAutoDefFunction* vCreateFunctionFromName(const std::string& name) const;
    virtual IGPAutoDefFunction* vCreateFunctionFromFormula(const std::string& formula) const;
    virtual IGPAutoDefFunction* vCreateFunctionFromNode(const GPTreeNode* node) const;
    
    GPTreeProducer(const GPFunctionDataBase* sys);
    virtual ~GPTreeProducer(){}
    void setFunctionDataBase(const GPFunctionDataBase* comsys);
    
    float getLargetVary() const {return mLargeVary;}
    const GPFunctionDataBase* getDataBase() const {return mDataBase;}
    void searchAllSequences(std::vector<std::vector<GPTreeADFPoint*> >& res, const std::vector<const IStatusType*>& outputType, const std::vector<const IStatusType*>& inputType, bool inputRepeat = true) const;
    GPTreeADF* searchOne(std::vector<std::vector<GPTreeADFPoint*> >& res, const std::vector<const IStatusType*>& outputType, const std::vector<const IStatusType*>& inputType, bool inputRepeat, int offset) const;//rate: 0.0-1.0, means the offset
private:
    const GPFunctionDataBase* mDataBase;
    GPProducerUtils mUtils;
    void _init();
    void _findMatchedFuncton(std::vector<std::vector<const GPProducerUtils::func*> >& warpOutput, const std::vector<const IStatusType*>& outputType, const std::vector<const IStatusType*>& inputType) const;
    
    float mLargeVary;
};
#endif
