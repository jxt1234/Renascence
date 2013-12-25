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

using namespace std;

struct pointData
{
    int functionId;
    int statusId;
    int childNumber;
};

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
    setDataBase(this);
    GP_Input inp;
    return IGPAutoDefFunction::run(inp);
}

void xmlGPLoader::reset()
{
    for (int i=0; i<mHandles.size(); ++i)
    {
        system_unload_lib(mHandles[i]);
    }
    mHandles.clear();
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

void xmlGPLoader::_getStatusFunc(const string& name, statusLoadMethod& _load, statusVaryMethod& _free)
{
    string loadName = name +"_load";
    string freeName = name +"_free";
    _load = (statusLoadMethod)system_find_func(mCurrentHandle, loadName.c_str());
    _free = (statusVaryMethod)system_find_func(mCurrentHandle, freeName.c_str());
    assert(NULL!=_load);
    assert(NULL!=_free);
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
        _getStatusFunc(name, _load, _free);
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
    mCurrentPoint->mStatus = status_loadSet(types, contents);
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
    statusLoadMethod load;
    statusVaryMethod _free;
    _getStatusFunc(name, load, _free);
    mStatusName.push_back(name);
    int type = status_allocType(0, name, NULL, _free, NULL, NULL, NULL, load);
    mStatusType.push_back(type);
    return type;
}

void* xmlGPLoader::findLib(string name)
{
    for (int i=0; i<mLibName.size(); ++i)
    {
        if (mLibName[i]==name)
        {
            return mHandles[i];
        }
    }
    void* handle = system_load_lib(name.c_str());
    assert(NULL!=handle);
    mLibName.push_back(name);
    mHandles.push_back(handle);
    return handle;
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
    computeFunction func = (computeFunction)system_find_func(mCurrentHandle, name.c_str());
    assert(NULL!=func);
    mFunctions.push_back(func);
    mFuncName.push_back(name);
    return mFuncName.size()-1;
}
void xmlGPLoader::_lib(xmlReader::package* p)
{
    string libName = p->attr[0];
    mCurrentHandle = findLib(libName);
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
