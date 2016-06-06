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
#include "core/GPParallelMachineSet.h"
#include "backend/GPSingleParallelMachine.h"
#include "platform/system_lib.h"
#include "GPStrings.h"

GPParallelMachineSet::GPParallelMachineSet()
{
    mHandle = new GPMultiTable;
}
GPParallelMachineSet::~GPParallelMachineSet()
{
}
void GPParallelMachineSet::addFunction(const GPTreeNode* node, IFunctionTable* table)
{
    const std::string& libName = node->name();
    if (NULL != table)
    {
        mHandle->addTable(table);
    }
    else
    {
        table = new system_lib(libName);
        mHandle->addTable(table);
        table->decRef();
    }
    PARALLELMACHINE_CREATE_FUNC func = mHandle->get<PARALLELMACHINE_CREATE_FUNC>(libName+"_" + GPStrings::FunctionDataBase_PARALLELMACHINE_CREATE_FUNC);
    GPASSERT(NULL!=func);
    mCreateFunctions.push_back(func);
    for (auto c : node->getChildren())
    {
        GPASSERT(mNamesMap.find(c->name()) == mNamesMap.end());
        mNames.push_back(c->name());
        mNamesMap.insert(std::make_pair(c->name(), func));
    }
}


IParallelMachine* GPParallelMachineSet::newMachine(const std::string& name)
{
    if (name == "basic")
    {
        return new GPSingleParallelMachine;
    }
    auto iter = mNamesMap.find(name);
    GPASSERT(iter != mNamesMap.end());
    return (iter->second)(name);
}
