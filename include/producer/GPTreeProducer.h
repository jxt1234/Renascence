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
#include "core/GPTreeADF.h"

class GPTreeProducer:public GPProducer
{
    public:
        virtual IGPAutoDefFunction* vCreateFunction(const std::vector<const IStatusType*>& outputType, const std::vector<const IStatusType*>& inputType, bool inputRepeat = true, bool random = false);
        virtual std::vector<IGPAutoDefFunction*> vCreateAllFunction(const std::vector<const IStatusType*>& outputType, const std::vector<const IStatusType*>& inputType, bool inputRepeat = true);
        virtual IGPAutoDefFunction* vCreateFunctionFromName(const std::string& name);
        virtual IGPAutoDefFunction* vCreateFunctionFromFormula(const std::string& formula);
        virtual IGPAutoDefFunction* vCreateFunctionFromIS(std::istream& is);

        GPTreeProducer():mDataBase(NULL){_init();}
        GPTreeProducer(const GPFunctionDataBase* sys){setFunctionDataBase(sys);_init();}
        virtual ~GPTreeProducer(){}
        void setFunctionDataBase(const GPFunctionDataBase* comsys);
        virtual void vMutate(IGPAutoDefFunction* tree) const;
    private:
        const GPFunctionDataBase* mDataBase;
        void _searchAllSequences(std::vector<std::vector<int> >& res, const std::vector<const IStatusType*>& outputType, const std::vector<const IStatusType*>& inputType, bool inputRepeat = true) const;
        void _findMatchedFuncton(std::vector<std::vector<int> >& warpOutput, const std::vector<const IStatusType*>& outputType) const;
        /*The output is the target function Id*/
        bool initGP(GPTreeADF* tree, const std::string& type, bool random = true);
        bool initGP(GPTreeADF* tree, const std::vector<int>& queue);
        void _init();

        float mLargeVary;
        float mStatusVary;
};
#endif
