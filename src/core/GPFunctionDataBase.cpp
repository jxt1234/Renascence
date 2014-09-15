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
#include <assert.h>
#include "core/funcStatusType.h"
#include <algorithm>
#include "system/system_lib.h"

using namespace std;


GPFunctionDataBase::GPFunctionDataBase()
{
}

void GPFunctionDataBase::loadXml(const char* file, IFunctionTable* table, std::ostream* print)
{
    ifstream is(file);
    assert(!is.fail());
    loadXml(is, table, print);
}
void GPFunctionDataBase::loadXml(std::istream& is, IFunctionTable* table, std::ostream* print)
{
    xmlFunctionLoader xmlLoader;
    xmlLoader.loadStream(is);
    if (NULL==table)
    {
        table = new system_lib(xmlLoader.libName);
        mHandle.push_back(table);
    }
    if (NULL!=print)
    {
        xmlLoader.print(*print);
    }
    this->loadFuncXml(xmlLoader, table);
}
std::vector<const GPFunctionDataBase::function*> GPFunctionDataBase::vSearchFunction(const IStatusType* t) const
{
    vector<const GPFunctionDataBase::function*> res;
    for (int i=0; i<mFunctionTable.size(); ++i)
    {
        const vector<const IStatusType*>& output = mFunctionTable[i]->outputType;
        if (find(output.begin(), output.end(), t)!=output.end())
        {
            res.push_back(mFunctionTable[i]);
        }
    }
    return res;
}

int GPFunctionDataBase::size() const
{
    return mFunctionTable.size();
}


GPFunctionDataBase::~GPFunctionDataBase()
{
    for (int i=0; i<mFunctionTable.size(); ++i)
    {
        delete mFunctionTable[i];
    }
    mFunctionTable.clear();
    for (int i=0; i<mTypes.size(); ++i)
    {
        delete mTypes[i];
    }
    for (int i=0; i<mHandle.size(); ++i)
    {
        delete mHandle[i];
    }
}


void GPFunctionDataBase::clear()
{
    for (int i=0; i<mFunctionTable.size(); ++i)
    {
        delete mFunctionTable[i];
    }
    mFunctionTable.clear();
}

vector<const IStatusType*> GPFunctionDataBase::loadStatus(const vector<xmlFunctionLoader::status>& sta, IFunctionTable* handle)
{
    vector<const IStatusType*> types;
    for (int i=0; i<sta.size(); ++i)
    {
        funcStatusType* type = new funcStatusType(sta[i].name, handle);
        mTypes.push_back(type);
        types.push_back(type);
    }
    return types;
}

void GPFunctionDataBase::loadFuncXml(xmlFunctionLoader& loader, IFunctionTable* table)
{
    vector<const IStatusType*> statusTypeId = loadStatus(loader.getStatus(), table);
    int offset = mFunctionTable.size();
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
        vector<const IStatusType*> newST;
        vector<const IStatusType*> newOT;
        vector<const IStatusType*> newIT;
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
            os <<fc->inputType[j]->name()<<" ";
        }
        os << endl;
        os << "The outputType is ";
        for (int j=0; j<fc->outputType.size(); ++j)
        {
            os <<fc->outputType[j]->name()<<" ";
        }
        os << endl;
        os << "The statusType is ";
        for (int j=0; j<fc->statusType.size(); ++j)
        {
            os <<fc->statusType[j]->name()<<" ";
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
    os << endl;
}
const IStatusType* GPFunctionDataBase::vQueryType(const std::string& name) const
{
    const IStatusType* res = NULL;
    for (int i=0; i<mTypes.size(); ++i)
    {
        if (mTypes[i]->name() == name)
        {
            res = mTypes[i];
            break;
        }
    }
    return res;
}
const GPFunctionDataBase::function* GPFunctionDataBase::vQueryFunction (const string& name) const
{
    for (int i=0; i<mFunctionTable.size(); ++i)
    {
        if (mFunctionTable[i]->name == name)
        {
            return mFunctionTable[i];
        }
    }
    return NULL;
}
const GPFunctionDataBase::function* GPFunctionDataBase::vQueryFunctionById (int id) const
{
    assert(id>=0&&id<mFunctionTable.size());
    return mFunctionTable[id];
}
