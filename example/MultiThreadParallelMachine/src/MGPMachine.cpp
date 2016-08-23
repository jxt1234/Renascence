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
#include "MGPExecutor.h"

IParallelMachine::Executor* MGPMachine::vPrepare(const GPParallelType* data, PARALLELTYPE type) const
{
    MGPExecutor* executor = new MGPExecutor(data, 4,  type);
    return executor;
}


IParallelMachine* libmthpGP_PARALLELMACHINE_CREATE_FUNC(const std::string& name)
{
    if (name == "thread")
    {
        return new MGPMachine;
    }
    return NULL;
}
