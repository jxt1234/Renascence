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

#include "computeSystem.h"
#include <assert.h>

#include "core/IDataBase.h"
#include "core/GPProducer.h"

class GenerateSystem:public GPProducer, public IRuntimeDataBase
{
    public:
        GenerateSystem():mComputeSystem(NULL){}
        GenerateSystem(computeSystem* sys){setComputeSystem(sys);}
        virtual IGPAutoDefFunction* vCreateFunction(const std::vector<int>& outputType, const std::vector<int>& inputType, bool inputRepeat = true, bool random = false);
        virtual std::vector<IGPAutoDefFunction*> vCreateAllFunction(const std::vector<int>& outputType, const std::vector<int>& inputType, bool inputRepeat = true);
        virtual IGPAutoDefFunction* vCreateFunctionFromName(const std::string& name);
        virtual computeFunction vGetCompute(int id);
        virtual int vQueryInputsNumber(int id);
        virtual int vQueryOutputNumber(int id);
        virtual ~GenerateSystem(){}
        void setComputeSystem(computeSystem* comsys);
        inline int getFuncId(const std::string& name){return mComputeSystem->vQueryFuncId(name);}
        void mutate(AbstractGP* tree);
        void freeStatus(AbstractGP* tree);
    protected:
        virtual IGPAutoDefFunction* vCreateADFFromGP(AbstractGP* gp) = 0;
        virtual void vSetInputNumber(AbstractGP* gp) {gp->setInputNumber(this);}
        virtual std::vector<int> searchSequence(int output, const std::vector<int>& input);
        virtual std::vector<std::vector<int> > searchAllSequence(int output, const std::vector<int>& input);
        virtual std::vector<int> searchRandSequence(int output, const std::vector<int>& input);
        virtual std::vector<int> searchType(const std::string& type);
        computeSystem* mComputeSystem;
    private:
        void _allocStatusForQueue(std::vector<int>& queue);
        void _findMatchedFuncton(std::vector<std::vector<int> >& warpOutput, const std::vector<int>& outputType);
        /*The output is the target function Id*/
        bool initGP(AbstractGP* tree, int output, const std::vector<int>& input, bool random = true);
        bool initGP(AbstractGP* tree, const std::string& type, bool random = true);
        bool initGP(AbstractGP* tree, const std::vector<int>& queue);
};
#endif
