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
#include "core/GeneticProgram.h"
#include <assert.h>

#include "core/IDataBase.h"

class GenerateSystem:public IFunctionDataBase
{
    public:
        GenerateSystem():mComputeSystem(NULL){}
        GenerateSystem(computeSystem* sys ):mComputeSystem(sys){}
        virtual computeFunction vGetCompute(int id);
        virtual ~GenerateSystem(){}
        std::vector<int> searchOneSequence();
        std::vector<int> searchOneWithOutput(int out);
        virtual std::vector<int> getRandSequence();
        virtual std::vector<int> getRandSequenceWithOutput(int outputFunctionId);
        inline void setComputeSystem(computeSystem* comsys){mComputeSystem = comsys;}
    protected:
        computeSystem* mComputeSystem;

};
#endif
