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
//Defines
struct statusSet
{
    vector<int> type;
    vector<void*> content;
};

//Private Values
struct statusType
{
    int size;
    string name;
    statusAllocMethod alloc;
    statusVaryMethod sfree;
    statusVaryMethod mutate;
    statusCopyMethod copy;
    statusPrintMethod print;
    statusLoadMethod load;
};

static vector<statusSet> gStatusSet;
static vector<statusType> gStatusType;
static vector<int> NULLVector;

static bool static_validId(int statusId)
{
    if (gStatusSet.size()<=statusId || 0 > statusId) return false;
    if (gStatusSet[statusId].content.empty()) return false;
    return true;
}

//API
bool status_init()
{
    status_clear();
    gStatusType.clear();
    return true;
}
int status_allocType(int size, string name, statusAllocMethod alloc, statusVaryMethod free, statusVaryMethod vary, statusCopyMethod copy, statusPrintMethod print, statusLoadMethod load)
{
    statusType t = {size, name, alloc, free, vary, copy, print, load};
    gStatusType.push_back(t);
    return gStatusType.size()-1;
}

int _findEmptySet()
{
    int pos = -1;
    //Find a position that can insert
    for (int i=0; i<gStatusSet.size(); ++i)
    {
        if (gStatusSet[i].content.empty())
        {
            pos = i;
            break;
        }
    }
    return pos;
}
int status_allocSet(const vector<int>& type)
{
    int pos = -1;
    if (type.empty()) return pos;
    pos = _findEmptySet();
    statusSet set;
    set.type = type;
    for (int i=0; i<type.size(); ++i)
    {
        statusType& t = gStatusType[type[i]];
        if (t.alloc)
        {
            set.content.push_back(t.alloc());
        }
        else
        {
            set.content.push_back(malloc(t.size));
        }
    }
    //Not found, alloc a new one
    if (-1==pos)
    {
        gStatusSet.push_back(set);
        pos = gStatusSet.size()-1;
    }
    else
    {
        gStatusSet[pos] = set;
    }
    return pos;
}

int status_loadSet(const std::vector<int>& type, std::vector<std::string>& contents)
{
    assert(contents.size() == type.size());
    assert(!contents.empty());
    statusSet set;
    set.type = type;
    for (int i=0; i<type.size(); ++i)
    {
        statusType& t = gStatusType[type[i]];
        if (t.load)
        {
            set.content.push_back(t.load(contents[i]));
        }
    }
    int pos = _findEmptySet();
    int result;
    if (-1==pos)
    {
        gStatusSet.push_back(set);
        result = gStatusSet.size() -1;
    }
    else
    {
        gStatusSet[pos] = set;
        result = pos;
    }
    return result;
}



const vector<int>& status_queryType(int statusId)
{
    if (static_validId(statusId))
    {
        return (gStatusSet[statusId].type);
    }
    return NULLVector;
}


bool status_freeSet(int statusId)
{
    if (!static_validId(statusId)) return false;
    statusSet& set = gStatusSet[statusId];
    for (int i=0; i< set.type.size(); ++i)
    {
        statusType& t = gStatusType[set.type[i]];
        if (t.sfree)
        {
            t.sfree(set.content[i]);
        }
    }
    set.type.clear();
    set.content.clear();
    return true;
}

vector<void*> status_queryContent(int statusId)
{
    vector<void*> NULLVoidVector;
    if (!static_validId(statusId)) return NULLVoidVector;
    return gStatusSet[statusId].content;
}

bool status_clear()
{
    if (gStatusSet.empty()) return false;
    for (int i=0; i<gStatusSet.size(); ++i)
    {
        status_freeSet(i);
    }
    return true;
}

bool status_varySet(int statusId)
{
    if (!static_validId(statusId)) return false;
    statusSet& s = gStatusSet[statusId];
    for (int i=0; i<s.type.size(); ++i)
    {
        statusType& t = gStatusType[s.type[i]];
        if(t.mutate)
        {
            t.mutate(s.content[i]);
        }
    }
    return true;
}
string status_printSet(int statusId)
{
    ostringstream result;
    if (!static_validId(statusId)) return "";
    statusSet& s = gStatusSet[statusId];
    for (int i=0; i<s.type.size(); ++i)
    {
        statusType& t = gStatusType[s.type[i]];
        result <<"<"<<t.name <<">"<<endl;
        if(t.print)
        {
            result << t.print(s.content[i]);
        }
        result <<endl<<"</"<<t.name <<">"<<endl;
    }
    return result.str();
}
int status_CopyAllocSet(int srcId)
{
    if (!static_validId(srcId)) return -1;
    statusSet& s = gStatusSet[srcId];
    int dstId = status_allocSet(s.type);
    status_CopySet(srcId, dstId);
    return dstId;
}
bool status_CopySet(int srcId, int dstId)
{
    if (!static_validId(srcId)) return false;
    status_freeSet(dstId);
    statusSet& s = gStatusSet[srcId];
    statusSet& d = gStatusSet[dstId];
    d.type = s.type;
    for (int i=0; i<s.type.size(); ++i)
    {
        statusType& t = gStatusType[s.type[i]];
        if (t.alloc)
        {
            d.content.push_back(t.alloc());
        }
        else
        {
            d.content.push_back(malloc(t.size));
        }
        if (t.copy)
        {
            t.copy(s.content[i], d.content[i]);
        }
    }
    return true;
}
