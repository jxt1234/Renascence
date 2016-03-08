/******************************************************************
 Copyright 2015, Jiang Xiao-tang
 
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

#include <algorithm>
#include <map>
#include <set>
#include "frontend/GPProducerUtils.h"
#include "math/carryArray.h"
#include "math/carryGroup.h"
using namespace std;


static void _setUpBasicFunction(GPProducerUtils::func* dst, const GPFunction* src)
{
    GPASSERT(dst->useChildrenInput.size() == src->inputType.size());
    dst->inputs.clear();
    dst->childrenInputs.clear();
    for (int i=0; i<dst->useChildrenInput.size(); ++i)
    {
        if (0 == dst->useChildrenInput[i])
        {
            dst->inputs.push_back(src->inputType[i]);
        }
        else
        {
            dst->childrenInputs.push_back(src->inputType[i]);
        }
    }
    dst->outputs = src->outputType;
    dst->basic = src;
}

static bool _validFunctions(const vector<const GPFunction*>& functionList)
{
    bool res = true;
    return res;
}


GPProducerUtils::func GPProducerUtils::func::create(FUNC f, bool clear)
{
    func res;
    int usechild = clear ? 1 : 0;
    res.useChildrenInput = vector<int>(f->inputType.size(), usechild);
    _setUpBasicFunction(&res, f);
    return res;
}

void GPProducerUtils::func::invalidate(func& f)
{
    _setUpBasicFunction(&f, f.basic);
}


GPProducerUtils::GPProducerUtils(const GPFunctionDataBase* base)
{
    mBase = base;
    auto functions = base->getAllFunctions();
    GPASSERT(_validFunctions(functions));
    map<FUNC, vector<func*>> funcmap;
    vector<GPPtr<func>> allfunctions;
    /*Create basic function, expand functions*/
    for (auto f : functions)
    {
        if (f->inputType.size()<=0)
        {
            GPPtr<func> mapfunc = new func;
            mapfunc->useChildrenInput = vector<int>(f->inputType.size(), 0);
            _setUpBasicFunction(mapfunc.get(), f);
            allfunctions.push_back(mapfunc);
            vector<func*> list(1, mapfunc.get());
            funcmap.insert(make_pair(f, list));
            continue;
        }
        carryGroup<int> group;
        for (int i=0; i<f->inputType.size(); ++i)
        {
            if (f->inputType[i]->name() != "double" && f->inputType[i]->name() != "String")
            {
                group.mBase.push_back(std::vector<int>{0,1});
            }
            else
            {
                group.mBase.push_back(std::vector<int>{0});
            }
        }
        vector<func*> list;
        group.reset();
        do
        {
            GPPtr<func> mapfunc = new func;
            mapfunc->useChildrenInput = group.current();
            _setUpBasicFunction(mapfunc.get(), f);
            allfunctions.push_back(mapfunc);
            list.push_back(mapfunc.get());
        }while (group.next());
        funcmap.insert(make_pair(f, list));
    }
    /*Assign Input, and filter invalid functions*/
    mFunctions.clear();
    for (auto f : allfunctions)
    {
        bool valid = true;
        do
        {
            if (f->childrenInputs.size()<=0)
            {
                break;
            }
            GPASSERT(f->tables.empty());
            carryGroup<const func*> group;
            for (int i=0; i<f->childrenInputs.size(); ++i)
            {
                auto pairs = _getFunctionsForOutput(f->childrenInputs[i], allfunctions);
                if (pairs.empty())
                {
                    valid = false;
                    break;
                }
                group.mBase.push_back(pairs);
            }
            if (!valid)
            {
                break;
            }
            group.reset();
            do
            {
                f->tables.push_back(group.current());
            } while(group.next());
        }while(0);
        if (valid)
        {
            mFunctions.push_back(f);
        }
    }
    _invalidateTable();
    GPASSERT(!mFunctions.empty());
}
GPProducerUtils::~GPProducerUtils()
{
}

void GPProducerUtils::_invalidateTable()
{
    set<const func*> validset;
    for (auto f : mFunctions)
    {
        validset.insert(f.get());
    }
    for (auto f : mFunctions)
    {
        auto cacheTable = f->tables;
        f->tables.clear();
        for (auto p : cacheTable)
        {
            bool valid = true;
            for (auto pf : p)
            {
                if (validset.find(pf) == validset.end())
                {
                    valid  = false;
                    break;
                }
            }
            if (valid)
            {
                f->tables.push_back(p);
            }
        }
    }
    for (auto f : mFunctions)
    {
        for (auto t : f->tables)
        {
            GPASSERT(t.size() == f->childrenInputs.size());
        }
    }
}


vector<const GPProducerUtils::func*> GPProducerUtils::getFunctionsForOutput(TYPE t) const
{
    return _getFunctionsForOutput(t, mFunctions);
}
vector<const GPProducerUtils::func*> GPProducerUtils::_getFunctionsForOutput(TYPE t, const std::vector<GPPtr<func>>& lists) const
{
    vector<const GPProducerUtils::func*> result;
    for (auto f : lists)
    {
        if (f->outputs.size() == 1 && t == f->outputs[0])
        {
            result.push_back(f.get());
        }
    }
    return result;
}
