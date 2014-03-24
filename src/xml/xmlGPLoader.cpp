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
#include "xml/xmlGPLoader.h"
#include "system/system_lib.h"
#include "core/status.h"
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <sstream>

using namespace std;

struct pointData
{
    int functionId;
    int statusId;
    int childNumber;
};

xmlGPLoader::~xmlGPLoader()
{
    if (mTableDestroy) delete mTable;
    for (int i=0; i<mStatusTypeFree.size(); ++i)
    {
        delete mStatusTypeFree[i];
    }
}
void xmlGPLoader::attributeUnflatten()
{
    if (NULL!=mAttributes)
    {
        AbstractGP::reset();
        mCurrentPoint = this;
        unFlattenUnit(mAttributes);
    }
}

GP_Output xmlGPLoader::run()
{
    class xmlGPLoader_ADF:public IGPAutoDefFunction
    {
        public:
            xmlGPLoader_ADF(xmlGPLoader* _m):m(_m){}
            ~xmlGPLoader_ADF(){}
            virtual GP_Output run(const GP_Input& inputs)
            {
                m->compute(m, m);
                return m->output();
            }
        private:
            xmlGPLoader* m;
    };
    xmlGPLoader_ADF exe(this);
    GP_Input inp;
    return exe.run(inp);
}

void xmlGPLoader::reset()
{
    mLibName.clear();
    mFuncName.clear();
    mFunctions.clear();
    mStatusType.clear();
    mStatusName.clear();
}

void xmlGPLoader::unFlattenUnit(xmlReader::package* p)
{
#define RUN(NAME, FUNC) \
    if (p->name == NAME) \
    {\
        FUNC(p);\
        return;\
    }
    RUN(GP_XmlString::node, _node);
    RUN(GP_XmlString::status, _status);
    RUN(GP_XmlString::lib, _lib);
    RUN(GP_XmlString::func, _func);
    RUN(GP_XmlString::children, _children);
    RUN(GP_XmlString::result, _result);
#undef RUN
}

void xmlGPLoader::_node(xmlReader::package* p)
{
    for (int i=0; i<p->children.size(); ++i)
    {
        unFlattenUnit(p->children[i]);
    }
}

bool xmlGPLoader::_getStatusFunc(const string& name, statusLoadMethod& _load, statusVaryMethod& _free)
{
    string loadName = name +"_load";
    string freeName = name +"_free";
    _load = (statusLoadMethod)(mTable->vGetFunction(loadName));
    _free = (statusVaryMethod)(mTable->vGetFunction(freeName));
    if (NULL == _load || NULL == _free) return false;
    return true;
}
/*Generate GP_Output, freeCallBack is type's free method.Though it's something like status, but are not the same*/
void xmlGPLoader::_result(xmlReader::package* p)
{
    if (p->children.size() == 0) return;
    GP_Output* output_save = new GP_Output;
    for (int i=0; i<p->children.size(); ++i)
    {
        GP_Output::GP_Unit unit;
        xmlReader::package* _p = p->children[i];
        string name = _p->name;
        string content;
        statusLoadMethod _load;
        statusVaryMethod _free;
        bool res = _getStatusFunc(name, _load, _free);
        if (!res)
        {
            GP_Output_clear(*output_save);
            delete output_save;
            return;
        }
        for (int j=0; j<_p->attr.size(); ++j)
        {
            content = content +" "+ _p->attr[j];
        }
        unit.content = _load(content);
        unit.freeCallBack = _free;
        (output_save->output).push_back(unit);
    }
    mCurrentPoint->mSave = (output_save);
}
void xmlGPLoader::_status(xmlReader::package* p)
{
    vector<int> types;
    vector<string> contents;
    if (p->children.size() == 0) return;
    for (int i=0; i<p->children.size(); ++i)
    {
        xmlReader::package* _p = p->children[i];
        string name = _p->name;
        string content;
        for (int j=0; j<_p->attr.size(); ++j)
        {
            content = content +" "+ _p->attr[j];
        }
        int type = findStatus(name);
        types.push_back(type);
        contents.push_back(content);
    }
    const IStatusType& t = queryType(types[0]);
    istringstream is(contents[0]);
    void* c = t.load(is);
    mCurrentPoint->mStatus = allocSet(types[0], c);
}

int xmlGPLoader::findStatus(string name)
{
    for (int i=0; i<mStatusName.size(); ++i)
    {
        if (name == mStatusName[i])
        {
            return mStatusType[i];
        }
    }
    /*Alloc status type*/
    IStatusType* newType = new funcStatusType(name, mTable);
    int type = addType(newType);
    mStatusName.push_back(name);
    mStatusType.push_back(type);
    mStatusTypeFree.push_back(newType);
    return type;
}

void xmlGPLoader::findLib(string name)
{
    /*Search If need to add this lib*/
    for (int i=0; i<mLibName.size(); ++i)
    {
        if (mLibName[i]==name)
        {
            return;
        }
    }
    system_multi_lib* table = dynamic_cast<system_multi_lib*>(mTable);
    assert(NULL!=table);
    table->addLib(name.c_str());
}
int xmlGPLoader::findFunc(string name)
{
    for (int i=0; i<mFuncName.size(); ++i)
    {
        if (mFuncName[i] == name)
        {
            return i;
        }
    }
    /*Add function*/
    computeFunction func = (computeFunction)(mTable->vGetFunction(name));
    assert(NULL!=func);
    mFunctions.push_back(func);
    mFuncName.push_back(name);
    return mFuncName.size()-1;
}
void xmlGPLoader::_lib(xmlReader::package* p)
{
    string libName = p->attr[0];
    findLib(libName);
}


void xmlGPLoader::_children(xmlReader::package* p)
{
    AbstractGP* back = mCurrentPoint;
    for (int i=0; i<p->children.size(); ++i)
    {
        mCurrentPoint = new AbstractGP;
        back->addPoint(mCurrentPoint);
        unFlattenUnit(p->children[i]);
    }
    mCurrentPoint = back;
}
void xmlGPLoader::_func(xmlReader::package* p)
{
    string name = p->attr[0];
    int id = findFunc(name);
    mCurrentPoint->mFunc = id;
}
