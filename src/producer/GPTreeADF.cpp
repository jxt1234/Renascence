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
}

GPTreeADFPoint::GPTreeADFPoint(const GPProducerUtils::func& func)
{
    mFunc = func;
    mFunc.tables.clear();
    vector<TYPEP> s = func.basic->statusType;
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
    const std::vector<const IStatusType*>& statustype = mFunc.basic->statusType;
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


GPTreeNode* GPTreeADFPoint::xmlPrint() const
{
    GPTreeNode* root = new GPTreeNode(GP_XmlString::func, mFunc.basic->name);
    GPPtr<GPTreeNode> status = new GPTreeNode(GP_XmlString::status, "");
    root->addChild(status);
    for (int j=0; j<mStatus.size(); ++j)
    {
        const IStatusType* s = mStatus[j]->type();
        std::ostringstream values;
        mStatus[j]->print(values);
        status->addChild(s->name(), values.str());
    }
    GPPtr<GPTreeNode> children = new GPTreeNode(GP_XmlString::children, "");
    root->addChild(children);
    for (int i=0; i<mChildren.size(); ++i)
    {
        GPTreeADFPoint* p = (GPTreeADFPoint*)(mChildren[i]);
        children->addChild(p->xmlPrint());
    }
    return root;
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


void GPTreeADFPoint::getinput(std::vector<const IStatusType*>& tlist) const
{
    tlist.insert(tlist.end(), (mFunc.inputs).begin(), (mFunc.inputs).end());
    for (int i=0; i<mChildren.size(); ++i)
    {
        GPTreeADFPoint* p = (GPTreeADFPoint*)mChildren[i];
        p->getinput(tlist);
    }
}

GPContents* GPTreeADFPoint::compute(GPContents* input, int& cur)
{
    GPContents outsideinputs;
    GPASSERT(!(mFunc.inputs.size() > 0 && mChildren.size() > 0));
    GPASSERT(input->size()>=(mFunc.inputs).size());
    for (int i=0; i<(mFunc.inputs).size(); ++i)
    {
        outsideinputs.push(input->contents[cur++]);
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
        }
        delete out;
    }
    /*For function that don't know inputType, just copy all contents to totalInputs*/
    GPContents totalInputs;
    if (mFunc.basic->inputType.size() <= 0)
    {
        for (int i=0; i<childreninputs.size(); ++i)
        {
            totalInputs.push(childreninputs.contents[i]);
        }
    }
    else
    {
        /*Merge outsideinputs and childreninputs*/
        int children_cur = 0;
        int outside_cur = 0;
        for (int i=0; i<mFunc.useChildrenInput.size(); ++i)
        {
            if (mFunc.useChildrenInput[i]>0)
            {
                totalInputs.push(childreninputs.contents[children_cur++]);
            }
            else
            {
                totalInputs.push(outsideinputs.contents[outside_cur++]);
            }
        }
        GPASSERT(outside_cur == outsideinputs.size());
        GPASSERT(children_cur == childreninputs.size());
    }
    //Get status
    for (int i=0; i<mStatus.size(); ++i)
    {
        totalInputs.push(mStatus[i]->content(), mStatus[i]->type());
    }
    GPContents* result;
    {
#ifdef DEBUG_TIMES
        GP_Clock c(__LINE__, (mFunc->name).c_str());
#endif
        result = mFunc.basic->basic(&totalInputs);
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

GPTreeNode* GPTreeADF::vSave() const
{
    GPASSERT(NULL!=mRoot);
    GPTreeNode* root = new GPTreeNode("GPTreeADF", "");
    root->addChild(mRoot->xmlPrint());
    return root;
}

GPContents* GPTreeADF::vRun(GPContents* inputs)
{
    GPASSERT(NULL!=mRoot);
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
            vector<vector<GPTreeADFPoint*> > queue;
            mProducer->searchAllSequences(queue, outputs, inputs);
            GPASSERT(!queue.empty());
            int n = GPRandom::mid(0, queue.size());
            auto q = queue[n];
            GPASSERT(1 == q.size());
            if (p != mRoot)
            {
                mRoot->replace(p, q[0]);
            }
            else
            {
                mRoot->decRef();
                mRoot = q[0];
                q[0]->addRef();
            }
            for (auto que : queue)
            {
                for (auto _q : que)
                {
                    _q->decRef();
                }
            }
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
    mOutputTypes = mRoot->mFunc.outputs;
    mRoot->getinput(res);
    mInputTypes = res;
}

