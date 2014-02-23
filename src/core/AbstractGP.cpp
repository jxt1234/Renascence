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
#include "core/AbstractGP.h"
#include "core/GP_XmlString.h"
#include <algorithm>
#include <sstream>
#include "utils/debug.h"
#include <assert.h>

using namespace std;
#define DO_CHILDREN_FUNC(func) \
    for (int i=0; i<mChildren.size();++i)\
    {\
        AbstractGP* p = dynamic_cast<AbstractGP*>(mChildren[i]);\
        p->func;\
    }

AbstractPoint* AbstractGP::AbstractGPCopy::copy(AbstractPoint* src)
{
    assert(NULL!=mBasic);
    AbstractGP* result = new AbstractGP;
    AbstractGP* s = dynamic_cast<AbstractGP*>(src);
    result->mFunc = s->mFunc;
    result->mStatus = mBasic->copySet(s->mStatus);
    result->mSave = NULL;
    return result;
}
void AbstractGP::reset()
{
    _reset();
    DO_CHILDREN_FUNC(reset());
}

AbstractGP::~AbstractGP()
{
    if (NULL!=mSave)
    {
        delete mSave;
        mSave = NULL;
    }
}

void AbstractGP::loadUnitFunction(vector<int>& result, int functionId, int statusId, int number)
{
    result.push_back(functionId);
    result.push_back(statusId);
    result.push_back(number);
}
void AbstractGP::save(IRuntimeDataBase* map, statusBasic* sta, const std::vector<int>& functionIds)
{
    if (functionIds.empty() || find(functionIds.begin(), functionIds.end(), mFunc)!=functionIds.end())
    {
        _reset();
        GP_Output output = up_compute(map, sta);
        mSave = new GP_Output;
        *mSave = output;
    }
    DO_CHILDREN_FUNC(save(map, sta, functionIds));
}

string AbstractGP::xmlPrint(IPrintDataBase* data, statusBasic* statusData)
{
    string funcName, libName;
    ostringstream res;
    res << "<"<< GP_XmlString::node<<">"<<endl;
    data->vQueryFunction(mFunc, funcName, libName);
    res<<"<"<<GP_XmlString::lib<<">"<<libName<<"</"<<GP_XmlString::lib<<">\n";
    res<<"<"<<GP_XmlString::func<<">"<<funcName<<"</"<<GP_XmlString::func<<">\n";
    /*If the point is saved, just print the value, assert that outputType's size should be empty or 1*/
    if (NULL!=mSave)
    {
        vector<void*> output;
        GP_Output_collect(output, *(mSave));
        vector<int> outputType;
        data->vQueryOutput(mFunc, outputType);
        if (!outputType.empty())
        {
            res << "<"<<GP_XmlString::result<<">"<<endl;
            const IStatusType& s = statusData->queryType(outputType[0]);
            res << "<" << s.name() <<">"<<endl;
            s.print(res, output[0]);
            res << endl<<"</" << s.name() <<">"<<endl;
            res << "</"<<GP_XmlString::result<<">"<<endl;
        }
    }
    if (mStatus >= 0)
    {
        res<<"<"<<GP_XmlString::status<<">\n";
        const IStatusType& s = statusData->queryTypeBySetId(mStatus);
        res << "<" << s.name() <<">"<<endl;
        void* content = statusData->queryContent(mStatus);
        s.print(res, content);
        res << endl<<"</" << s.name() <<">"<<endl;
        res<<"</"<<GP_XmlString::status<<">\n";
    }
    res <<"<"<< GP_XmlString::children<<">"<<endl;
    for (int i=0; i<mChildren.size(); ++i)
    {
        AbstractGP* p = dynamic_cast<AbstractGP*>(mChildren[i]);
        res << p->xmlPrint(data, statusData);
    }
    res <<"</"<< GP_XmlString::children<<">"<<endl;
    res << "</"<< GP_XmlString::node<<">"<<endl;
    return res.str();
}

std::vector<int> AbstractGP::getStatus()
{
    vector<int> res;
    list<AbstractGP*> cacheQueue;
    cacheQueue.push_back(this);
    while(!cacheQueue.empty())
    {
        AbstractGP* current = cacheQueue.front();
        res.push_back(current->mStatus);
        for (int i=0; i<current->mChildren.size(); ++i)
        {
            AbstractGP* p = dynamic_cast<AbstractGP*>(current->mChildren[i]);
            assert(NULL!=p);
            cacheQueue.push_back(p);
        }
        cacheQueue.pop_front();
    }
    return res;
}

void AbstractGP::replacePoint(const std::vector<int> &numbers, int& cur)
{
    //Clear all children, status and result
    _reset();
    for (int i=0; i<mChildren.size(); ++i)
    {
        delete mChildren[i];
    }
    mChildren.clear();
    //Wide-search
    list<AbstractGP*> cacheQueue;
    cacheQueue.push_back(this);
    while(!cacheQueue.empty())
    {
        AbstractGP* current = cacheQueue.front();
        current->mFunc = numbers[cur++];
        current->mStatus = numbers[cur++];
        int pointNumber = numbers[cur++];
        for (int i=0; i < pointNumber; ++i)
        {
            AbstractGP* input = new AbstractGP;
            current->mChildren.push_back(input);
            cacheQueue.push_back(input);
        }
        cacheQueue.pop_front();
    }
}

void AbstractGP::compute(IRuntimeDataBase* map, statusBasic* sta)
{
    _reset();
    GP_Output result = up_compute(map, sta);
    mSave = new GP_Output;
    *mSave = result;
}

void AbstractGP::_reset()
{
    if (NULL!=mSave)
    {
        GP_Output_clear(*mSave);
        delete mSave;
        mSave = NULL;
    }
}

void AbstractGP::input(GP_Input& input, int& cur)
{
    assert(input.size() >= cur+mInputs.size());
    for (int i=0; i<mInputs.size(); ++i)
    {
        mInputs[i] = input[cur++];
    }
    for (int i=0; i<mChildren.size(); ++i)
    {
        AbstractGP* p = dynamic_cast<AbstractGP*>(mChildren[i]);
        p->input(input, cur);
    }
}

GP_Output AbstractGP::output()
{
    GP_Output result;
    if (NULL!=mSave)
    {
        result = *mSave;
    }
    /*The memory of contents shoud be clean outside*/
    delete mSave;
    mSave = NULL;
    return result;
}

vector<int> AbstractGP::setInputNumber(IRuntimeDataBase* map)
{
    vector<int> funcId;
    int num = map->vQueryInputsNumber(mFunc);
    mInputs.clear();
    if (num > 0)
    {
        funcId.push_back(mFunc);
    }
    for (int i=0; i<num; ++i)
    {
        mInputs.push_back(NULL);
    }
    for (int i=0; i<mChildren.size(); ++i)
    {
        AbstractGP* p = dynamic_cast<AbstractGP*>(mChildren[i]);
        vector<int> child = p->setInputNumber(map);
        for (int j=0;j<child.size(); ++j)
        {
            funcId.push_back(child[j]);
        }
    }
    return funcId;
}

GP_Output AbstractGP::up_compute(IRuntimeDataBase* map, statusBasic* sta)
{
    GP_Output result;
    if (NULL!=mSave)
    {
        result = *mSave;
        for (int i=0; i<result.output.size(); ++i)
        {
            result.output[i].freeCallBack = NULL;
        }
        return result;
    }
    computeFunction comp = map->vGetCompute(mFunc);
    vector<GP_Output::GP_Unit> inputMap;
    vector<void*> children;
    //Get Inputs from childern point
    for (int i=0; i<mChildren.size(); ++i)
    {
        AbstractGP* p = dynamic_cast<AbstractGP*>(mChildren[i]);
        assert(NULL!=p);
        GP_Output out = p->up_compute(map, sta);
        vector<GP_Output::GP_Unit>& output_unit = out.output;
        for (int j=0; j<output_unit.size(); ++j)
        {
            inputMap.push_back(output_unit[j]);
            children.push_back(output_unit[j].content);
        }
    }
    //Get status
    vector<void*> constValue;
    if (-1 != mStatus)
    {
        constValue.push_back(sta->queryContent(mStatus));
    }
    vector<void*> totalInputs;
    totalInputs.insert(totalInputs.begin(), constValue.begin(), constValue.end());
    totalInputs.insert(totalInputs.begin(), mInputs.begin(), mInputs.end());
    totalInputs.insert(totalInputs.begin(), children.begin(), children.end());
    {
#ifdef DEBUG_TIMES
        GP_Clock c(mFunc);
#endif
        result = comp(totalInputs);
    }
    //Free All children' memory
    for (int i=0; i < inputMap.size(); ++i)
    {
        GP_Output::GP_Unit& out = inputMap[i];
        if (NULL!=out.freeCallBack)
        {
            out.freeCallBack(out.content);
        }
    }
    return result;
}
