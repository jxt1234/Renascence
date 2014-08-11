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
#include "core/GPTreeADF.h"
#include "core/GP_XmlString.h"
#include <algorithm>
#include <sstream>
#include <fstream>
#include "utils/debug.h"
#include <assert.h>
//#define DEBUG_TIMES
//#define DEBUG_XML

using namespace std;

GPTreeADFPoint::GPTreeADFPoint()
{
    mFunc = NULL;
}
GPTreeADFPoint::~GPTreeADFPoint()
{
    for (int i=0; i<mStatus.size(); ++i)
    {
        SAFE_UNREF(mStatus[i]);
    }
}


void GPTreeADFPoint::xmlPrint(std::ostream& res)
{
    res << "<"<< GP_XmlString::node<<">"<<endl;
    res<<"<"<<GP_XmlString::lib<<">"<<mFunc->libName<<"</"<<GP_XmlString::lib<<">\n";
    res<<"<"<<GP_XmlString::func<<">"<<mFunc->name<<"</"<<GP_XmlString::func<<">\n";
    res<<"<"<<GP_XmlString::status<<">\n";
    for (int j=0; j<mStatus.size(); ++j)
    {
        const IStatusType& s = mStatus[j]->type();
        res << "<" << s.name() <<">"<<endl;
        void* content = mStatus[j]->content();
        s.print(res, content);
        res << endl<<"</" << s.name() <<">"<<endl;
    }
    res<<"</"<<GP_XmlString::status<<">\n";
    res <<"<"<< GP_XmlString::children<<">"<<endl;
    for (int i=0; i<mChildren.size(); ++i)
    {
        GPTreeADFPoint* p = (GPTreeADFPoint*)(mChildren[i]);
        p->xmlPrint(res);
    }
    res <<"</"<< GP_XmlString::children<<">"<<endl;
    res << "</"<< GP_XmlString::node<<">"<<endl;
}
AbstractPoint* GPTreeADFPoint::GPTreeADFCopy::copy(AbstractPoint* src)
{
    GPTreeADFPoint* s = (GPTreeADFPoint*)(src);
    GPTreeADFPoint* result = new GPTreeADFPoint;
    result->mFunc = s->mFunc;
    vector<GPStatusContent*>& ss = s->mStatus;
    for (int i=0; i<ss.size(); ++i)
    {
        GPStatusContent* c = new GPStatusContent(*(ss[i]));
        (result->mStatus).push_back(c);
    }
    return result;
}



void GPTreeADFPoint::replacePoint(const std::vector<int> &numbers, const GPFunctionDataBase* base)
{
    assert(numbers.size()>0);
    int cur=0;
    _replacePoint(numbers, cur, base);
}
void GPTreeADFPoint::_replacePoint(const std::vector<int> &numbers, int& cur, const GPFunctionDataBase* base)
{
    assert(numbers.size()>0);
    assert(numbers.size()%3==0);
    //Clear all children, status and result
    for (int i=0; i<mChildren.size(); ++i)
    {
        delete mChildren[i];
    }
    for (int i=0; i<mStatus.size(); ++i)
    {
        SAFE_UNREF(mStatus[i]);
    }
    mChildren.clear();
    mStatus.clear();
    //Wide-search
    list<GPTreeADFPoint*> cacheQueue;
    cacheQueue.push_back(this);
    while(!cacheQueue.empty())
    {
        GPTreeADFPoint* current = cacheQueue.front();
        current->mFunc = base->getDetailFunction(numbers[cur++]);
        /*Init status*/
        cur++;//Status be none
        const vector<const IStatusType*>& t = current->mFunc->statusType;
        for (int i=0; i<t.size(); ++i)
        {
            (current->mStatus).push_back(new GPStatusContent(t[i]));
        }

        /*Init all childern*/
        int pointNumber = numbers[cur++];
        for (int i=0; i < pointNumber; ++i)
        {
            GPTreeADFPoint* input = new GPTreeADFPoint;
            current->mChildren.push_back(input);
            cacheQueue.push_back(input);
        }
        cacheQueue.pop_front();
    }
}

void GPTreeADFPoint::getinput(std::vector<const IStatusType*>& tlist) const
{
    tlist.insert(tlist.end(), (mFunc->inputType).begin(), (mFunc->inputType).end());
    for (int i=0; i<mChildren.size(); ++i)
    {
        GPTreeADFPoint* p = (GPTreeADFPoint*)mChildren[i];
        p->getinput(tlist);
    }
}


std::vector<const IStatusType*> GPTreeADF::vGetOutputs() const
{
    assert(NULL!=mRoot);
    return mRoot->mFunc->outputType;
}
std::vector<const IStatusType*> GPTreeADF::vGetInputs() const
{
    assert(NULL!=mRoot);
    vector<const IStatusType*> res;
    mRoot->getinput(res);
    return res;
}


GP_Output GPTreeADFPoint::compute(const GP_Input& input, int& cur)
{
    GP_Output result;
    computeFunction comp = mFunc->basic;
    vector<GP_Output::GP_Unit> inputMap;
    vector<void*> children;
    vector<void*> inputs;
    for (int i=0; i<(mFunc->inputType).size(); ++i)
    {
        inputs.push_back(input[cur++]);
    }
    //Get Inputs from childern point
    for (int i=0; i<mChildren.size(); ++i)
    {
        GPTreeADFPoint* p = (GPTreeADFPoint*)(mChildren[i]);
        assert(NULL!=p);
        GP_Output out = p->compute(input, cur);
        vector<GP_Output::GP_Unit>& output_unit = out.output;
        for (int j=0; j<output_unit.size(); ++j)
        {
            inputMap.push_back(output_unit[j]);
            children.push_back(output_unit[j].content);
        }
    }
    //Get status
    vector<void*> constValue;
    for (int i=0; i<mStatus.size(); ++i)
    {
        void* constValue_unit= mStatus[i]->content();
        constValue.push_back(constValue_unit);
    }
    vector<void*> totalInputs;
    totalInputs.insert(totalInputs.begin(), constValue.begin(), constValue.end());
    totalInputs.insert(totalInputs.begin(), inputs.begin(), inputs.end());
    totalInputs.insert(totalInputs.begin(), children.begin(), children.end());
    {
#ifdef DEBUG_TIMES
        GP_Clock c(__LINE__, (mFunc->name).c_str());
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

GPTreeADF::GPTreeADF()
{
    mRoot = new GPTreeADFPoint;
}
GPTreeADF::GPTreeADF(GPTreeADFPoint* root)
{
    assert(NULL!=root);
    mRoot = root;
}
GPTreeADF::~GPTreeADF()
{
    if (NULL!=mRoot) delete mRoot;
}
GPTreeADFPoint* GPTreeADF::find(float rate)
{
    assert(0.0<=rate && rate<1.0);
    vector<AbstractPoint*> nodes = mRoot->display();
    int n = rate*nodes.size();
    GPTreeADFPoint* p = (GPTreeADFPoint*)nodes[n];
    return p;
}

void GPTreeADF::loadUnitFunction(vector<int>& result, int functionId, int statusId, int number)
{
    result.push_back(functionId);
    result.push_back(statusId);
    result.push_back(number);
}

void GPTreeADF::save(std::ostream& res)
{
    assert(NULL!=mRoot);
    mRoot->xmlPrint(res);
}

void GPTreeADF::load(std::istream& is)
{
}
GP_Output GPTreeADF::run(const GP_Input& inputs)
{
    assert(NULL!=mRoot);
#ifdef DEBUG_XML
    ofstream os("GPTreeADF.xml");
    this->save(os);
    os.close();
#endif
    int cur = 0;
    return mRoot->compute(inputs, cur);
}
