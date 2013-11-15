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

using namespace std;
#define DO_CHILDREN_FUNC(func) \
    for (int i=0; i<mChildren.size();++i)\
    {\
        AbstractGP* p = dynamic_cast<AbstractGP*>(mChildren[i]);\
        p->func;\
    }

AbstractPoint* AbstractGP::AbstractGPCopy::copy(AbstractPoint* src)
{
    AbstractGP* result = new AbstractGP;
    AbstractGP* s = dynamic_cast<AbstractGP*>(src);
    result->mFunc = s->mFunc;
    result->mStatus = status_CopyAllocSet(s->mStatus);
    result->mSave = NULL;
    return result;
}
void AbstractGP::reset()
{
    _reset();
    DO_CHILDREN_FUNC(reset());
}
AbstractGP::AbstractGP(const AbstractGP& gp)
{
    mFunc = gp.mFunc;
    mStatus = gp.mStatus;
    AbstractGPCopy c;
    for (int i=0; i<gp.mChildren.size(); ++i)
    {
        addPoint(deepCopy((gp.mChildren[i]), &c));
    }
}

void AbstractGP::save(IFunctionDataBase* map, const std::vector<int>& functionIds)
{
    if (functionIds.empty() || find(functionIds.begin(), functionIds.end(), mFunc)!=functionIds.end())
    {
        _reset();
        mSave = new GP_Output;
        *mSave = up_compute(map);
    }
    DO_CHILDREN_FUNC(save(map, functionIds));
}

string AbstractGP::xmlPrint(IDataBase* data)
{
    string funcName, libName;
    ostringstream res;
    res << "<"<< GP_XmlString::node<<">"<<endl;
    data->vQueryFunction(mFunc, funcName, libName);
    res<<"<"<<GP_XmlString::lib<<">"<<libName<<"</"<<GP_XmlString::lib<<">\n";
    res<<"<"<<GP_XmlString::func<<">"<<funcName<<"</"<<GP_XmlString::func<<">\n";
    /*If the point is saved, just print the value*/
    if (NULL!=mSave)
    {
        res << "<"<<GP_XmlString::result<<">"<<endl;
        vector<void*> output;
        GP_Output_collect(output, *(mSave));
        vector<int> outputType;
        data->vQueryOutput(mFunc, outputType);
        status_printSetWithType(output, outputType, res);
        res << "</"<<GP_XmlString::result<<">"<<endl;
    }
    if (mStatus >= 0)
    {
        res<<"<"<<GP_XmlString::status<<">\n";
        res<<status_printSet(mStatus);
        res<<"</"<<GP_XmlString::status<<">\n";
    }
    res <<"<"<< GP_XmlString::children<<">"<<endl;
    for (int i=0; i<mChildren.size(); ++i)
    {
        AbstractGP* p = dynamic_cast<AbstractGP*>(mChildren[i]);
        res << p->xmlPrint(data);
    }
    res <<"</"<< GP_XmlString::children<<">"<<endl;
    res << "</"<< GP_XmlString::node<<">"<<endl;
    return res.str();
}

void AbstractGP::replacePoint(const std::vector<int> &numbers, int& cur)
{
    for (int i=0; i<mChildren.size(); ++i)
    {
        delete mChildren[i];
    }
    mChildren.clear();
    status_freeSet(mStatus);
    mFunc = numbers[cur++];
    mStatus = numbers[cur++];
    int n = numbers[cur++];
    for (int i=0; i<n; ++i)
    {
        AbstractGP* p = new AbstractGP;
        p->replacePoint(numbers, cur);
        mChildren.push_back(p);
    }
}
void AbstractGP::operator=(const AbstractGP& gp)
{
    for (int i=0; i<mChildren.size(); ++i)
    {
        delete mChildren[i];
    }
    mChildren.clear();
    mFunc = gp.mFunc;
    mStatus = gp.mStatus;
    mSave = NULL;
    AbstractGPCopy c;
    for (int i=0; i<gp.mChildren.size(); ++i)
    {
        addPoint(deepCopy((gp.mChildren[i]), &c));
    }
}

void AbstractGP::compute(IFunctionDataBase* map)
{
    _reset();
    GP_Output result = up_compute(map);
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

void AbstractGP::input(GP_Input& input)
{
    //TODO Add Input
}

GP_Output AbstractGP::output()
{
    GP_Output result;
    if (NULL!=mSave)
    {
        result = *mSave;
    }
    return result;
}

AbstractGP::~AbstractGP()
{
    if (NULL!=mSave)
    {
        delete mSave;
    }
    status_freeSet(mStatus);
}

GP_Output AbstractGP::up_compute(IFunctionDataBase* map)
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
        GP_Output out = p->up_compute(map);
        vector<GP_Output::GP_Unit>& output_unit = out.output;
        for (int j=0; j<output_unit.size(); ++j)
        {
            inputMap.push_back(output_unit[j]);
            children.push_back(output_unit[j].content);
        }
    }
    vector<void*> constValue;
    if (-1 != mStatus)
    {
        constValue = status_queryContent(mStatus);
    }
    {
#ifdef DEBUG_TIMES
        GP_Clock c(mFunc);
#endif
        result = comp(children, constValue);
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
