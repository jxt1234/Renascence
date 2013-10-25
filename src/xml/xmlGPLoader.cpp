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
        GeneticProgram::clear();
        mCurrentPoint = new GeneticPoint;
        mRoot = mCurrentPoint;
        unFlattenUnit(mAttributes);
    }
    vector<int> Queue = getPointQueue(mRoot);
#ifdef DEBUG_XML_GP
    cout <<Queue.size()<<endl;
    for (int i=0; i<Queue.size(); ++i)
    {
        cout <<Queue[i]<<" ";
    }
    cout <<endl;
#endif
}

GP_Output xmlGPLoader::run()
{
    return compute(mFunctions);
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
#undef RUN
}

void xmlGPLoader::_node(xmlReader::package* p)
{
    for (int i=0; i<p->children.size(); ++i)
    {
        unFlattenUnit(p->children[i]);
    }
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
    mCurrentPoint->statusId = status_loadSet(types, contents);
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
    string loadName = name +"_load";
    string freeName = name +"_free";
    statusLoadMethod load = (statusLoadMethod)system_find_func(mCurrentHandle, loadName.c_str());
    statusVaryMethod _free = (statusVaryMethod)system_find_func(mCurrentHandle, freeName.c_str());
    assert(NULL!=load);
    assert(NULL!=_free);
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
    GeneticPoint* back = mCurrentPoint;
    for (int i=0; i<p->children.size(); ++i)
    {
        mCurrentPoint = new GeneticPoint;
        back->children.push_back(mCurrentPoint);
        unFlattenUnit(p->children[i]);
    }
    mCurrentPoint = back;
}
void xmlGPLoader::_func(xmlReader::package* p)
{
    string name = p->attr[0];
    int id = findFunc(name);
    mCurrentPoint->functionId = id;
}
