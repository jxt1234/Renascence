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
#include <iostream>
#include <fstream>
#include "system/computeSystem.h"
#include "utils/debug.h"
#include "math/carryArray.h"
#include <assert.h>
#include "core/funcStatusType.h"

#define FUNCTION_TABLE_FILE_DEFAULT "db/function.table"

computeSystem::computeSystem()
{
}

int computeSystem::vQueryFuncId(const std::string& funcName)
{
    int id = -1;
    for (int i=0; i<mFunctionTable.size(); ++i)
    {
        if (mFunctionTable[i]->name == funcName)
        {
            id = i;
        }
    }
    return id;
}
const std::vector<std::vector<int> >& computeSystem::getAvailableFunctionInputs(int functionId)
{
    static vector<vector<int> > nullVector;
    if (functionId >=0 && functionId < mFunctionTable.size())
    {
        return (mFunctionTable[functionId])->fixTable;
    }
    return nullVector;
}

computeSystem::~computeSystem()
{
    clear();
}

void computeSystem::vQueryFunction(int id, std::string& name, std::string& libName)
{
    name = mFunctionTable[id]->name;
    libName = mFunctionTable[id]->libName;
}
void computeSystem::vQueryOutput(int functionId, std::vector<int>& output)
{
    output = mFunctionTable[functionId]->outputType;
}
computeFunction computeSystem::getFunction(int id)
{
    if (id < mFunctionTable.size() && 0<=id)
    {
        return mFunctionTable[id]->basic;
    }
    return NULL;
}

void computeSystem::clear()
{
    for (int i=0; i<mFunctionTable.size(); ++i)
    {
        delete mFunctionTable[i];
    }
    mFunctionTable.clear();
}

vector<int> computeSystem::loadStatus(const vector<xmlFunctionLoader::status>& sta, IFunctionTable* handle)
{
    vector<int> typeId;
    for (int i=0; i<sta.size(); ++i)
    {
#define FINDFUNC(type, subname) type subname = (type)handle->vGetFunction(sta[i].name+#subname);
        FINDFUNC(statusAllocMethod, _alloc);
        FINDFUNC(statusVaryMethod, _free);
        FINDFUNC(statusVaryMethod, _vary);
        FINDFUNC(statusCopyMethod, _copy);
        FINDFUNC(statusPrintMethod, _print);
        FINDFUNC(statusLoadMethod, _load);
#undef FINDFUNC
        int id = status_allocType(0, sta[i].name, _alloc, _free, _vary, _copy, _print, _load);
        typeId.push_back(id);
    }
    return typeId;
}

void computeSystem::loadFuncXml(xmlFunctionLoader& loader, IFunctionTable* table)
{
    vector<int> statusTypeId = loadStatus(loader.getStatus(), table);
    int offset = mFunctionTable.size();
    mOutputId.push_back(loader.mFit);
    const vector<xmlFunctionLoader::function>& functions = loader.getFunc();
    for (int i=0; i<functions.size(); ++i)
    {
        const xmlFunctionLoader::function& f = functions[i];
        computeSystem::function* fc = new computeSystem::function;
        fc->name = f.name;
        fc->libName = loader.libName;
        /*Load function handle*/
        computeFunction func = (computeFunction)table->vGetFunction(f.name);
        assert(NULL!=func);
        fc->basic = func;
        /*Load function status*/
        vector<int> statusType = f.statusType;
        vector<int> outputType = f.outputType;
        vector<int> newST;
        vector<int> newOT;
        for (int j=0; j<statusType.size(); ++j)
        {
            int cur = statusType[j];
            newST.push_back(statusTypeId[cur]);
        }
        for (int j=0; j<outputType.size(); ++j)
        {
            int cur = outputType[j];
            newOT.push_back(statusTypeId[cur]);
        }
        fc->statusType = newST;
        fc->outputType = newOT;
        /*Load function inputs*/
        vector<vector<int> > newCombo = f.inputs;
        for (int j=0; j<newCombo.size(); ++j)
        {
            for (int k=0; k<newCombo[j].size(); ++k)
            {
                newCombo[j][k]+=offset;
            }
        }
        fc->fixTable = newCombo;
        mFunctionTable.push_back(fc);
    }
    mInputId = loader.mInputs;
}


int computeSystem::allocateStatus(int id)
{
    if (id < 0 || id >= mFunctionTable.size()) return -1;
    return status_allocSet(mFunctionTable[id]->statusType);
}

void computeSystem::print()
{
}
