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
#include "core/status.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils/debug.h"
#include <execinfo.h>
#include <sstream>
#include <iostream>
#include <assert.h>
using namespace std;

static IStatusType gEmptyType(string("NULL"));

void statusBasic::clearStatusSet()
{
    for (int i=0; i<mContents.size(); ++i)
    {
        statusBasic::content& c = mContents[i];
        (c.type)->sfree(c.data);
    }
    mContents.clear();
}

statusBasic::~statusBasic()
{
    for (int i=0; i<mDeleteType.size(); ++i)
    {
        delete mDeleteType[i];
    }
}

const IStatusType& statusBasic::queryTypeBySetId(int id)
{
    if (id < 0 || id >= mContents.size())
    {
        return gEmptyType;
    }
    return *(mContents[id].type);
}
int statusBasic::queryType(const std::string& name)
{
    int res = -1;
    for (int i=0; i<mType.size(); ++i)
    {
        if (mType[i]->name() == name)
        {
            res = i;
            break;
        }
    }
    return res;
}


/*Currently, I use assert to avoid the excepetion*/
int statusBasic::addType(IStatusType* type, bool del)
{
    assert(NULL!=type);
    int id = mType.size();
    mType.push_back(type);
    if (del) mDeleteType.push_back(type);
    return id;
}


int statusBasic::allocSet(int type, void* content)
{
    if(type < 0 || type >= mType.size()) return -1;
    IStatusType* t = mType[type];
    return allocSet(t, content);
}
int statusBasic::allocSet(IStatusType* t, void* content)
{
    assert(NULL!=t);
    /*Alloc status content*/
    statusBasic::content c;
    if (NULL == content) content = t->salloc();
    c.type = t;
    c.data = content;
    /*Find position and put it*/
    int pos;
    for (pos = 0; pos < mContents.size(); ++pos)
    {
        if (mContents[pos].type == &gEmptyType)
        {
            mContents[pos] = c;
            break;
        }
    }
    //The End
    if (pos == mContents.size())
    {
        mContents.push_back(c);
    }
    return pos;

}

void* statusBasic::queryContent(int id)
{
    if(id < 0 || id >=mContents.size()) return NULL;
    return mContents[id].data;
}

int statusBasic::copySet(int id)
{
    if(id < 0 || id >=mContents.size()) return -1;
    IStatusType* t = mContents[id].type;
    void* src = mContents[id].data;
    assert((&gEmptyType) != t);
    int res = allocSet(t);
    t->copy(src, mContents[res].data);
    return res;
}
const IStatusType& statusBasic::queryType(int typeId)
{
    int id = typeId;
    if(id < 0 || id >=mType.size()) return gEmptyType;
    return *(mType[typeId]);
}

int statusBasic::freeSet(int id)
{
    if(id < 0 || id >=mContents.size()) return -1;
    statusBasic::content& c = mContents[id];
    (c.type)->sfree(c.data);
    c.type = &gEmptyType;
    c.data = NULL;
    return id;
}
int statusBasic::mutateSet(int id)
{
    if(id < 0 || id >=mContents.size()) return -1;
    statusBasic::content& c = mContents[id];
    (c.type)->mutate(c.data);
    return id;
}
