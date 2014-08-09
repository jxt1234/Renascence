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
#include "core/GPFunctionDataBase.h"
#include "utils/debug.h"
#include "math/carryArray.h"
#include <assert.h>
#include "core/funcStatusType.h"
#include <algorithm>

using namespace std;


GPFunctionDataBase::GPFunctionDataBase()
{
}
const GPFunctionDataBase::function& GPFunctionDataBase::getDetailFunction(int id)
{
    assert(id >=0 && id < mFunctionTable.size());
    return *(mFunctionTable[id]);
}

void GPFunctionDataBase::vQueryStatus(int id, std::string& name, std::string& libName)
{
}

std::vector<int> GPFunctionDataBase::getOutputFunctions(int typeId)
{
    if (0>typeId) return mOutputId;
    vector<int> res;
    for (int i=0; i<mFunctionTable.size(); ++i)
    {
        const vector<int>& output = mFunctionTable[i]->outputType;
        if (find(output.begin(), output.end(), typeId)!=output.end())
        {
            res.push_back(i);
        }
    }
    return res;
}
int GPFunctionDataBase::vQueryFuncId(const std::string& funcName)
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
const std::vector<std::vector<int> >& GPFunctionDataBase::getAvailableFunctionInputs(int functionId)
{
    static vector<vector<int> > nullVector;
    if (functionId >=0 && functionId < mFunctionTable.size())
    {
        return (mFunctionTable[functionId])->fixTable;
    }
    return nullVector;
}

GPFunctionDataBase::~GPFunctionDataBase()
{
    clear();
}

void GPFunctionDataBase::vQueryFunction(int id, std::string& name, std::string& libName)
{
    assert(id < mFunctionTable.size());
    name = mFunctionTable[id]->name;
    libName = mFunctionTable[id]->libName;
}
void GPFunctionDataBase::vQueryOutput(int functionId, std::vector<int>& output)
{
    output = mFunctionTable[functionId]->outputType;
}
computeFunction GPFunctionDataBase::getFunction(int id)
{
    if (id < mFunctionTable.size() && 0<=id)
    {
        return mFunctionTable[id]->basic;
    }
    return NULL;
}

int GPFunctionDataBase::getStatusId(int id)
{
    if (id < mFunctionTable.size() && 0<=id)
    {
        if (!mFunctionTable[id]->statusType.empty())
        {
            return mFunctionTable[id]->statusType[0];
        }
    }
    return -1;
}

void GPFunctionDataBase::clear()
{
    for (int i=0; i<mFunctionTable.size(); ++i)
    {
        delete mFunctionTable[i];
    }
    mFunctionTable.clear();
}

vector<int> GPFunctionDataBase::loadStatus(const vector<xmlFunctionLoader::status>& sta, IFunctionTable* handle, statusBasic* stadata)
{
    vector<int> typeId;
    for (int i=0; i<sta.size(); ++i)
    {
        funcStatusType* type = new funcStatusType(sta[i].name, handle);
        int id = stadata->addType(type, true);
        typeId.push_back(id);
    }
    return typeId;
}

void GPFunctionDataBase::loadFuncXml(xmlFunctionLoader& loader, IFunctionTable* table, statusBasic* stadata)
{
    vector<int> statusTypeId = loadStatus(loader.getStatus(), table, stadata);
    int offset = mFunctionTable.size();
    mOutputId.push_back(loader.mFit);
    const vector<xmlFunctionLoader::function>& functions = loader.getFunc();
    for (int i=0; i<functions.size(); ++i)
    {
        const xmlFunctionLoader::function& f = functions[i];
        GPFunctionDataBase::function* fc = new GPFunctionDataBase::function;
        fc->name = f.name;
        fc->libName = loader.libName;
        /*Load function handle*/
        computeFunction func = (computeFunction)table->vGetFunction(f.name);
        assert(NULL!=func);
        fc->basic = func;
        /*Load function status*/
        vector<int> statusType = f.statusType;
        vector<int> outputType = f.outputType;
        vector<int> inputType = f.inputType;
        vector<int> newST;
        vector<int> newOT;
        vector<int> newIT;
        for (int j=0; j<inputType.size(); ++j)
        {
            int cur = inputType[j];
            newIT.push_back(statusTypeId[cur]);
        }
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
        fc->inputType = newIT;
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


void GPFunctionDataBase::print(ostream& os)
{
    for (int i=0; i<mFunctionTable.size(); ++i)
    {
        GPFunctionDataBase::function* fc = mFunctionTable[i];
        os << i << ": "<<fc->name<<" : "<<fc->libName<<endl;
        os << "Required inputType is ";
        for (int j=0; j<fc->inputType.size(); ++j)
        {
            os <<fc->inputType[j]<<" ";
        }
        os << endl;
        os << "The outputType is ";
        for (int j=0; j<fc->outputType.size(); ++j)
        {
            os <<fc->outputType[j]<<" ";
        }
        os << endl;
        os << "The statusType is ";
        for (int j=0; j<fc->statusType.size(); ++j)
        {
            os <<fc->statusType[j]<<" ";
        }
        os << endl;
        vector<vector<int> >& combo = fc->fixTable;
        for (int x=0; x<combo.size(); ++x)
        {
            for (int y=0; y<combo[x].size(); ++y)
            {
                os << combo[x][y]<<" ";
            }
            os << endl;
        }
        os << endl;
    }
    os << "Output: ";
    for (int i=0; i<mOutputId.size(); ++i)
    {
        os << mOutputId[i] <<" ";
    }
    os << endl;
    os << "Input: ";
    for (int i=0; i<mInputId.size(); ++i)
    {
        os << mInputId[i] <<" ";
    }
    os << endl;
}
