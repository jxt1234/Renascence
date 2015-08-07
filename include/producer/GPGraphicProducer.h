/******************************************************************
   Copyright 2014, Jiang Xiao-tang

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
#ifndef PRODUCER_GPGRAPHICPRODUCER_H
#define PRODUCER_GPGRAPHICPRODUCER_H
#include "core/GPFunctionDataBase.h"
#include "core/GPProducer.h"
class GPGraphicProducer:public GPProducer
{
    public:
        virtual IGPAutoDefFunction* vCreateFunction(const std::vector<const IStatusType*>& outputType, const std::vector<const IStatusType*>& inputType, bool inputRepeat = true, bool random = false) const;
        virtual std::vector<IGPAutoDefFunction*> vCreateAllFunction(const std::vector<const IStatusType*>& outputType, const std::vector<const IStatusType*>& inputType, bool inputRepeat = true) const;
        virtual IGPAutoDefFunction* vCreateFunctionFromName(const std::string& name) const;
        virtual IGPAutoDefFunction* vCreateFunctionFromFormula(const std::string& formula) const;
        virtual IGPAutoDefFunction* vCreateFunctionFromNode(const GPTreeNode* node) const;
        virtual void vMutate(IGPAutoDefFunction* tree) const;
        GPGraphicProducer(const GPFunctionDataBase* base);
        virtual ~GPGraphicProducer();
    private:
        const GPFunctionDataBase* mDataBase;
        float mStatusVary;
};
#endif
