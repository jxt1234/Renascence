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
#include "MGPMachine.h"
#include "MGPCreator.h"
#include "MGPExecutor.h"

std::pair<IParallelMachine::Creator*, IParallelMachine::Executor*> MGPMachine::vGenerate(const GPParallelType* data, PARALLELTYPE type) const
{
    MGPExecutor* executor = new MGPExecutor(data->pContext, data->sFuncInfo.formula, data->sConditionInfo.sConditionFormula, 4,  type, data->mOutputKey);
    MGPCreator* creator = new MGPCreator(data->mOutputKey);
    
    return std::make_pair(creator, executor);
}


IParallelMachine* libmthpGP_PARALLELMACHINE_CREATE_FUNC(const std::string& name)
{
    if (name == "thread")
    {
        return new MGPMachine;
    }
    return NULL;
}
