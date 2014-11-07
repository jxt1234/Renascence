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
#include <utils/debug.h>
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
    GPASSERT(!is.fail());
    loadXml(is, table, print);
}
void GPFunctionDataBase::loadXml(std::istream& is, IFunctionTable* table, std::ostream* print)
{
    _clear();
    xmlReader reader;
    const xmlReader::package* root = reader.loadStream(is);
    if (NULL==table)
    {
        table = new system_lib(root->name);
        mHandle.push_back(table);
    }
    const vector<xmlReader::package*>& functions = root->children;
    /*First time construct all functions*/
    for (int i=0; i<functions.size(); ++i)
    {
        const xmlReader::package* func = functions.at(i);
        GPASSERT(NULL!=func);
        computeFunction f = (computeFunction)(table->vGetFunction(func->name));
        GPASSERT(NULL!=f);//TODO throw error when no function in the table
        function* warpf = new function;
        warpf->name = func->name;
        warpf->basic = f;
        mFunctionTable.push_back(warpf);
    }
    for (int i=0; i<functions.size(); ++i)
    {
        this->_addFunction(mFunctionTable.at(i), functions.at(i), table);
    }
    if (print)
    {
        this->print(*print);
    }
}

void GPFunctionDataBase::_addFunction(GPFunctionDataBase::function* warpf, const xmlReader::package* func, IFunctionTable* table)
{
    for (int i=0; i<func->children.size(); ++i)
    {
        const xmlReader::package* cur = (func->children).at(i);
        if (cur->name == "inputs")
        {
            vector<int> input;
            for (int j=0; j<cur->attr.size(); ++j)
            {
                int inp = _findFunction(cur->attr[j]);
                if (-1==inp)
                {
                    input.clear();
                    break;
                }
                input.push_back(inp);
            }
            if (!input.empty())
            {
                (warpf->fixTable).push_back(input);
            }
        }
        else if(cur->name == "status")
        {
            for (int j=0; j<cur->attr.size(); ++j)
            {
                const IStatusType* sta = _findAndLoadStatus(cur->attr[j], table);
                GPASSERT(NULL!=sta);//FIXME
                (warpf->statusType).push_back(sta);
            }
        }
        else if(cur->name == "output")
        {
            for (int j=0; j<cur->attr.size(); ++j)
            {
                const IStatusType* sta = _findAndLoadStatus(cur->attr[j], table);
                GPASSERT(NULL!=sta);//FIXME
                (warpf->outputType).push_back(sta);
            }
        }
        else if (cur->name == "inputType")
        {
            for (int j=0; j<cur->attr.size(); ++j)
            {
                const IStatusType* sta = _findAndLoadStatus(cur->attr[j], table);
                GPASSERT(NULL!=sta);//FIXME
                (warpf->inputType).push_back(sta);
            }
        }
    }
}

int  GPFunctionDataBase::_findFunction(const std::string& name)
{
    int res = -1;
    for (int i=0; i<mFunctionTable.size(); ++i)
    {
        if (mFunctionTable.at(i) -> name == name)
        {
            res = i;
            break;
        }
    }
    return res;
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
    _clear();
}


void GPFunctionDataBase::_clear()
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
    mTypes.clear();
    for (int i=0; i<mHandle.size(); ++i)
    {
        delete mHandle[i];
    }
    mHandle.clear();
}

const IStatusType* GPFunctionDataBase::_findAndLoadStatus(const std::string& name, IFunctionTable* handle)
{
    IStatusType* t = NULL;
    for (int i=0; i<mTypes.size(); ++i)
    {
        if (mTypes.at(i)->name() == name)
        {
            t = mTypes.at(i);
            break;
        }
    }
    if (NULL == t)
    {
        t = new funcStatusType(name, handle);
        mTypes.push_back(t);
    }
    return t;
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
    GPASSERT(id>=0&&id<mFunctionTable.size());
    return mFunctionTable[id];
}
