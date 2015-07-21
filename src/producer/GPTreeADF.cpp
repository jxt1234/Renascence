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
#include "core/GP_XmlString.h"
#include "producer/GPTreeADF.h"
#include "producer/GPTreeProducer.h"
#include <algorithm>
#include <sstream>
#include <fstream>
#include "utils/debug.h"
#include "utils/GPRandom.h"
//#define DEBUG_TIMES
//#define DEBUG_XML

using namespace std;
typedef const IStatusType* TYPEP;

GPTreeADFPoint::GPTreeADFPoint()
{
    mFunc = NULL;
}

GPTreeADFPoint::GPTreeADFPoint(const GPFunctionDataBase::function* func)
{
    GPASSERT(NULL!=func);
    mFunc = func;
    vector<TYPEP> s = func->statusType;
    for (int i=0; i<s.size(); ++i)
    {
        mStatus.push_back(new GPStatusContent(s[i]));
    }
}
GPTreeADFPoint::~GPTreeADFPoint()
{
    for (int i=0; i<mStatus.size(); ++i)
    {
        SAFE_UNREF(mStatus[i]);
    }
}

void GPTreeADFPoint::initStatus(const std::vector<std::istream*>& statusInput)
{
    GPASSERT(statusInput.size() == mStatus.size());
    const std::vector<const IStatusType*>& statustype = mFunc->statusType;
    GPASSERT(statusInput.size() == statustype.size());
    for (int i=0; i<mStatus.size(); ++i)
    {
        if (NULL==statusInput[i]) continue;
        istream& is = *(statusInput[i]);
        GPStatusContent* s= mStatus[i];
        AutoStorage<double> _v(s->size());
        double* v = _v.get();
        for (int j=0; j<s->size(); ++j)
        {
            is >> v[j];
        }
        s->setValue(v, s->size());
    }
}


void GPTreeADFPoint::xmlPrint(std::ostream& res) const
{
    res << "<"<< GP_XmlString::node<<">"<<endl;
    res<<"<"<<GP_XmlString::lib<<">"<<mFunc->shortname<<"</"<<GP_XmlString::lib<<">\n";
    res<<"<"<<GP_XmlString::func<<">"<<mFunc->name<<"</"<<GP_XmlString::func<<">\n";
    res<<"<"<<GP_XmlString::status<<">\n";
    for (int j=0; j<mStatus.size(); ++j)
    {
        const IStatusType* s = mStatus[j]->type();
        res << "<" << s->name() <<">"<<endl;
        mStatus[j]->print(res);
        res << endl<<"</" << s->name() <<">"<<endl;
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
    GPASSERT(numbers.size()>0);
    int cur=0;
    _replacePoint(numbers, cur, base);
}
void GPTreeADFPoint::_replacePoint(const std::vector<int> &numbers, int& cur, const GPFunctionDataBase* base)
{
    GPASSERT(numbers.size()>0);
    GPASSERT(numbers.size()%3==0);
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
        current->mFunc = base->vQueryFunctionById((numbers[cur++]));
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



GPContents* GPTreeADFPoint::compute(GPContents* input, int& cur)
{
    computeFunction comp = mFunc->basic;
    GPContents totalInputs;
    GPContents outinputs;
    GPASSERT(input->size()>=(mFunc->inputType).size());
    for (int i=0; i<(mFunc->inputType).size(); ++i)
    {
        outinputs.push(input->contents[cur++]);
    }
    GPContents childreninputs;
    //Get Inputs from childern point
    for (int i=0; i<mChildren.size(); ++i)
    {
        GPTreeADFPoint* p = (GPTreeADFPoint*)(mChildren[i]);
        GPASSERT(NULL!=p);
        GPContents* out = p->compute(input, cur);
        const auto& output_unit = out->contents;
        for (int j=0; j<output_unit.size(); ++j)
        {
            childreninputs.push(output_unit[j]);
            totalInputs.push(output_unit[j]);
        }
        delete out;
    }
    //Get status
    for (int i=0; i<mStatus.size(); ++i)
    {
        totalInputs.push(mStatus[i]->content(), mStatus[i]->type());
    }
    //Merge outside inputs
    for (int i=0; i<outinputs.size(); ++i)
    {
        totalInputs.push(outinputs.contents[i]);
    }

    //totalInputs.insert(totalInputs.begin(), constValue.begin(), constValue.end());
    //totalInputs.insert(totalInputs.begin(), inputs.begin(), inputs.end());
    //totalInputs.insert(totalInputs.begin(), children.begin(), children.end());
    GPContents* result;
    {
#ifdef DEBUG_TIMES
        GP_Clock c(__LINE__, (mFunc->name).c_str());
#endif
        result = comp(&totalInputs);
    }
    //Free All children' memory
    childreninputs.clear();
    return result;
}

GPTreeADF::GPTreeADF(GPTreeADFPoint* root, const GPTreeProducer* p)
{
    GPASSERT(NULL!=p);
    GPASSERT(NULL!=root);
    mRoot = root;
    mProducer = p;
    _refreshInputsAndOutputs();
}
GPTreeADF::~GPTreeADF()
{
    if (NULL!=mRoot) delete mRoot;
}
GPTreeADFPoint* GPTreeADF::find(float rate)
{
    GPASSERT(0.0<=rate && rate<1.0);
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

void GPTreeADF::vSave(std::ostream& res) const
{
    GPASSERT(NULL!=mRoot);
    mRoot->xmlPrint(res);
}

GPContents* GPTreeADF::vRun(GPContents* inputs)
{
    GPASSERT(NULL!=mRoot);
#ifdef DEBUG_XML
    ofstream os("GPTreeADF.xml");
    this->vSave(os);
    os.close();
#endif
    int cur = 0;
    return mRoot->compute(inputs, cur);
}

IGPAutoDefFunction* GPTreeADF::vCopy() const
{
    GPTreeADFPoint::GPTreeADFCopy c;
    GPTreeADFPoint* root = mRoot;
    GPTreeADFPoint* p = (GPTreeADFPoint*)AbstractPoint::deepCopy(root, &c);
    return new GPTreeADF(p, mProducer);
}
int GPTreeADF::vMap(GPPtr<GPParameter> para)
{
    GPASSERT(NULL!=mRoot);
    vector<AbstractPoint*> allpoints = mRoot->display();
    vector<GPStatusContent*> allcontents;
    int sum = 0;
    for (int i=0; i<allpoints.size(); ++i)
    {
        GPTreeADFPoint* p = (GPTreeADFPoint*)allpoints[i];
        vector<GPStatusContent*>& status = p->mStatus;
        for (int j=0; j<status.size(); ++j)
        {
            sum+=status[j]->size();
            allcontents.push_back(status[j]);
        }
    }
    if (NULL!=para.get())
    {
        GPASSERT(sum == para->size());
        const PFLOAT* ps = para->get();
        for (int i=0; i<allcontents.size(); ++i)
        {
            GPStatusContent* c = allcontents[i];
            AutoStorage<double> _values(c->size());
            double* v = _values.get();
            for (int j=0; j<c->size(); ++j)
            {
                v[j] = ps[j];
            }
            ps+=c->size();
            c->setValue(v, c->size());
        }
    }
    return sum;
}

void GPTreeADF::vMutate()
{
    float pos = GPRandom::rate();
    GPTreeADFPoint* p = find(pos);
    /*Replace or mutate status*/
    if (GPRandom::rate() < mProducer->getLargetVary())
    {
        const vector<const IStatusType*> outputs = p->func()->outputType;
        //TODO If outputs is empty, Use function name to vary
        if (!outputs.empty())
        {
            vector<const IStatusType*> inputs;
            p->pGetInputs(inputs);
            vector<vector<int> > queue;
            mProducer->searchAllSequences(queue, outputs, inputs);
            GPASSERT(!queue.empty());
            int n = GPRandom::mid(0, queue.size());
            p->replacePoint(queue[n], mProducer->getDataBase());
            _refreshInputsAndOutputs();
        }
    }
    const std::vector<GPStatusContent*>& mStatus = p->status();
    for (int i=0; i<mStatus.size(); ++i)
    {
        mStatus[i]->mutate();
    }
}

void GPTreeADF::_refreshInputsAndOutputs()
{
    GPASSERT(NULL!=mRoot);
    vector<const IStatusType*> res;
    mOutputTypes = mRoot->mFunc->outputType;
    mRoot->getinput(res);
    mInputTypes = res;
}

