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
#include "GPStrings.h"
#include "backend/GPTreeADF.h"
#include "backend/GPTreeProducer.h"
#include <algorithm>
#include <sstream>
#include <fstream>
#include "utils/GPDebug.h"
//#define DEBUG_TIMES
//#define DEBUG_XML

using namespace std;
typedef const IStatusType* TYPEP;
GPTreeADFPoint::GPTreeADFPoint(const GPFunctionDataBase::function* func, int inputpos)
{
    GPASSERT(NULL != func || inputpos >= 0);
    mInputPos = inputpos;
    mFunc = func;
    if (NULL == func)
    {
        return;
    }
    vector<TYPEP> s = func->statusType;
    for (int i=0; i<s.size(); ++i)
    {
        mStatus.push_back(new GPStatusContent(s[i]));
    }
}
GPTreeADFPoint::~GPTreeADFPoint()
{
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
        GPStatusContent* s= mStatus[i].get();
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
    if (NULL == mFunc)
    {
        std::ostringstream out;
        out << mInputPos;
        return new GPTreeNode(GPStrings::GPTreeADF_INPUTS, out.str());
    }
    GPTreeNode* root = new GPTreeNode(GPStrings::GPTreeADF_FUNCTION, mFunc->name);
    GPPtr<GPTreeNode> status = new GPTreeNode(GPStrings::GPTreeADF_STATUS, "");
    root->addChild(status);
    for (int j=0; j<mStatus.size(); ++j)
    {
        const IStatusType* s = mStatus[j]->type();
        std::ostringstream values;
        mStatus[j]->print(values);
        status->addChild(s->name(), values.str());
    }
    GPPtr<GPTreeNode> children = new GPTreeNode(GPStrings::GPTreeADF_CHILDREN, "");
    root->addChild(children);
    GPASSERT(mChildren.size() == mFunc->inputType.size());
    for (int i=0; i<mChildren.size(); ++i)
    {
        GPTreeADFPoint* p = (GPTreeADFPoint*)(mChildren[i]);
        children->addChild(p->xmlPrint());
    }
    return root;
}
GPTreeADFPoint* GPTreeADFPoint::xmlLoad(const GPTreeNode* node, const GPFunctionDataBase* base)
{
    GPASSERT(NULL!=node);
    GPTreeADFPoint* res = NULL;
    if (node->name() == GPStrings::GPTreeADF_FUNCTION)
    {
        res = new GPTreeADFPoint(base->vQueryFunction(node->attr()), -1);
        for (auto c : node->getChildren())
        {
            if (c->name() == GPStrings::GPTreeADF_STATUS)
            {
                std::vector<istream*> streams;
                for (auto cc : c->getChildren())
                {
                    std::istringstream* is = new std::istringstream(cc->attr());
                    streams.push_back(is);
                }
                res->initStatus(streams);
                for (auto is : streams)
                {
                    delete is;
                }
            }
            else if (c->name() == GPStrings::GPTreeADF_CHILDREN)
            {
                for (auto cc : c->getChildren())
                {
                    res->addPoint(xmlLoad(cc.get(), base));
                }
            }
        }
    }
    else if (node->name() == GPStrings::GPTreeADF_INPUTS)
    {
        int inputpos = -1;
        std::istringstream is(node->attr());
        is >> inputpos;
        res = new GPTreeADFPoint(NULL, inputpos);
    }
    GPASSERT(NULL!=res);
    return res;
}


GPContents* GPTreeADFPoint::compute(GPContents* input)
{
    GPContents outsideinputs;
    GPASSERT(NULL!=mFunc);
    GPContents childreninputs;
    GPContents totalInputs;
    //Get Inputs from childern point
    for (int i=0; i<mChildren.size(); ++i)
    {
        GPTreeADFPoint* p = GPCONVERT(GPTreeADFPoint, mChildren[i]);
        GPASSERT(NULL!=p);
        if (p->mFunc == NULL)
        {
            totalInputs.push(input->contents[p->mInputPos]);
            continue;
        }
        GPContents* out = p->compute(input);
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
    GPContents* result;
    {
#ifdef DEBUG_TIMES
        GP_Clock c(__LINE__, (mFunc->name).c_str());
#endif
        result = mFunc->basic(&totalInputs);
    }
    //Free All children' memory
    childreninputs.clear();
    return result;
}

GPTreeADF::GPTreeADF(GPTreeADFPoint* root)
{
    GPASSERT(NULL!=root);
    mRoot = root;
}
GPTreeADF::~GPTreeADF()
{
    if (NULL!=mRoot) mRoot->decRef();
}
GPTreeADFPoint* GPTreeADF::find(float rate)
{
    GPASSERT(0.0<=rate && rate<1.0);
    vector<const GPAbstractPoint*> nodes = mRoot->display();
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
    return mRoot->compute(inputs);
}

int GPTreeADF::vMap(GPParameter* para)
{
    GPASSERT(NULL!=mRoot);
    vector<const GPAbstractPoint*> allpoints = mRoot->display();
    vector<GPStatusContent*> allcontents;
    int sum = 0;
    for (int i=0; i<allpoints.size(); ++i)
    {
        GPTreeADFPoint* p = (GPTreeADFPoint*)allpoints[i];
        vector<GPPtr<GPStatusContent>>& status = p->mStatus;
        for (int j=0; j<status.size(); ++j)
        {
            sum+=status[j]->size();
            allcontents.push_back(status[j].get());
        }
    }
    if (NULL!=para)
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
