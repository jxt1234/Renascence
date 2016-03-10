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
#include <sstream>
#include "GPStrings.h"
#include "core/GPFunctionDataBase.h"
#include "utils/GPDebug.h"
#include <algorithm>
#include "platform/system_lib.h"
#include "utils/GPStringHelper.h"

using namespace std;

std::vector<const GPFunction*> GPFunctionDataBase::getAllFunctions() const
{
    std::vector<const GPFunction*> result;
    for (int i=0; i<mFunctionTable.size(); ++i)
    {
        result.push_back(mFunctionTable[i]);
    }
    return result;
}


std::vector<const IStatusType*> GPFunctionDataBase::queryType(const std::string& typelist)
{
    std::vector<const IStatusType*> results;
    std::istringstream is(typelist);
    std::string name;
    while (is >> name)
    {
        results.push_back(this->vQueryType(name));
    }
    return results;
}

GPFunctionDataBase::GPFunctionDataBase()
{
    mHandle = new GPMultiTable;
    mTypes.push_back(gDefaultDoubleType);
    mTypes.push_back(gDefaultStringType);
}

void GPFunctionDataBase::loadXml(GPStream* is, IFunctionTable* table, std::ostream* print)
{
    xmlReader reader;
    const GPTreeNode* root = reader.loadStream(is);
    _addInfo(root, table, print);
}
void GPFunctionDataBase::_addInfo(const GPTreeNode* root, IFunctionTable* table, std::ostream* print)
{
    if (NULL==table)
    {
        table = new system_lib(root->name());
        mHandle->addTable(table, true);
    }
    else
    {
        mHandle->addTable(table, false);
    }
    TYPECREATER create = mHandle->get<TYPECREATER>(GPStrings::FunctionDataBase_STATUS_CREATOR);
    GPASSERT(NULL!=create);
    mTypeCreateFuncs.push_back(create);
    auto functions = root->getChildren();
    /*First time construct all functions*/
    auto offset = mFunctionTable.size();
    for (size_t i=0; i<functions.size(); ++i)
    {
        const GPTreeNode* func = functions.at(i).get();
        GPASSERT(NULL!=func);
        computeFunction f = (computeFunction)(table->vGetFunction(func->name()));
        if (NULL == f)
        {
            FUNC_PRINT_ALL((func->name()).c_str(), s);
            GPASSERT(NULL!=f);//TODO throw error when no function in the table
        }
        GPFunction* warpf = new GPFunction;
        warpf->name = func->name();
        warpf->shortname = func->name();//Default
        warpf->basic = f;
        mFunctionTable.push_back(warpf);
    }
    /*Then add info for all functions*/
    for (size_t i=0; i<functions.size(); ++i)
    {
        this->_addFunction(mFunctionTable.at(i+offset), functions.at(i).get());
    }
    if (print)
    {
        this->print(*print);
    }
}

void GPFunctionDataBase::_addFunction(GPFunction* warpf, const GPTreeNode* func)
{
    for (auto cur : func->getChildren())
    {
        if (cur->name() == GPStrings::FunctionDataBase_SHORTNAME)
        {
            warpf->shortname = cur->attr();
        }
        else if(cur->name() == GPStrings::FunctionDataBase_STATUS)
        {
            auto attrs = GPStringHelper::divideString(cur->attr());
            for (auto attr : attrs)
            {
                const IStatusType* sta = _findAndLoadStatus(attr);
                GPASSERT(NULL!=sta);//FIXME
                (warpf->statusType).push_back(sta);
            }
        }
        else if(cur->name() == GPStrings::FunctionDataBase_OUTPUT)
        {
            auto attrs = GPStringHelper::divideString(cur->attr());
            for (auto attr : attrs)
            {
                const IStatusType* sta = _findAndLoadStatus(attr);
                GPASSERT(NULL!=sta);//FIXME
                (warpf->outputType).push_back(sta);
            }
        }
        else if (cur->name() == GPStrings::FunctionDataBase_INPUT)
        {
            auto attrs = GPStringHelper::divideString(cur->attr());
            for (auto attr : attrs)
            {
                const IStatusType* sta = _findAndLoadStatus(attr);
                GPASSERT(NULL!=sta);//FIXME
                (warpf->inputType).push_back(sta);
            }
        }
        else if (cur->name() == GPStrings::FunctionDataBase_COMPLETE_FLAG)
        {
            auto attrs = GPStringHelper::divideString(cur->attr());
            for (auto attr : attrs)
            {
                if (attr == "True")
                {
                    warpf->inputNeedComplete.push_back(true);
                }
                else if (attr == "False")
                {
                    warpf->inputNeedComplete.push_back(false);
                }
                else
                {
                    GPASSERT(0);
                }
            }
        }
    }
}

int GPFunctionDataBase::_findFunction(const std::string& name)
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

std::vector<const GPFunction*> GPFunctionDataBase::vSearchFunction(const IStatusType* t) const
{
    vector<const GPFunction*> res;
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
    mTypes.clear();
    mHandle = new GPMultiTable;
}

const IStatusType* GPFunctionDataBase::_findAndLoadStatus(const std::string& name)
{
    const IStatusType* t = NULL;
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
        for (auto create : mTypeCreateFuncs)
        {
            t = create(name);
            if (NULL!=t)
            {
                mTypes.push_back(t);
                break;
            }
        }
    }
    if (NULL == t)
    {
        FUNC_PRINT_ALL(name.c_str(), s);
    }
    GPASSERT(NULL!=t);
    return t;
}

void GPFunctionDataBase::print(ostream& os)
{
    for (int i=0; i<mFunctionTable.size(); ++i)
    {
        GPFunction* fc = mFunctionTable[i];
        os << i << ": "<<fc->name<<" : "<<fc->shortname<<endl;
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
    if (NULL == res)
    {
        FUNC_PRINT_ALL(name.c_str(), s);
        GPASSERT(NULL!=res);//FIXME
    }
    return res;
}
const GPFunction* GPFunctionDataBase::vQueryFunctionByShortName(const std::string& name) const
{
    for (int i=0; i<mFunctionTable.size(); ++i)
    {
        if (mFunctionTable[i]->shortname == name)
        {
            return mFunctionTable[i];
        }
    }
    return NULL;
}

const GPFunction* GPFunctionDataBase::vQueryFunction (const string& name) const
{
    for (int i=0; i<mFunctionTable.size(); ++i)
    {
        if (mFunctionTable[i]->name == name)
        {
            return mFunctionTable[i];
        }
    }
    GPPRINT("Function %s Not Found!!\n", name.c_str());
    GPASSERT(0);
    return NULL;
}
