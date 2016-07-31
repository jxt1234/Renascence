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
#ifndef CORE_GPPARALLELMACHINESET_H
#define CORE_GPPARALLELMACHINESET_H
#include "lowlevelAPI/IParallelMachine.h"
#include "lowlevelAPI/IFunctionTable.h"
#include "utils/GPTreeNode.h"
#include "GPMultiTable.h"
#include "head.h"
#include <map>
class GPParallelMachineSet:public GPRefCount
{
public:
    GPParallelMachineSet();
    virtual ~GPParallelMachineSet();
    void addFunction(const GPTreeNode* node, IFunctionTable* table);

    const std::vector<std::string> listAllMachines() const
    {
        return mNames;
    }

    const IParallelMachine* getMachine(const std::string& name);
private:
    std::vector<std::string> mNames;
    std::map<std::string, IParallelMachine*> mNamesMap;
    GPPtr<GPMultiTable> mHandle;
};
#endif
