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
#include <list>
#include <algorithm>
#include "recurse_tree.h"
using namespace std;

vector<int> computePoint::filter(const vector<vector<const GPProducerUtils::func*> >& combo, const vector<const GPProducerUtils::func*>& output)
{
    vector<int> result;
    for (int i=0; i<combo.size(); ++i)
    {
        const vector<const GPProducerUtils::func*>& comboUnit = combo[i];
        bool fit = true;
        const vector<const IStatusType*>& input_obtained = mInput;
        for (int j=0; j<comboUnit.size(); ++j)
        {
            /*Check if it contained old output*/
            if (find(output.begin(), output.end(), comboUnit[j]) != output.end())
            {
                fit = false;
                break;
            }
            /*Check if it contained unobtained input*/
            const GPProducerUtils::func* f = comboUnit[j];
            for (int k=0; k<f->inputs.size(); ++k)
            {
                const IStatusType* inpId = f->inputs[k];
                if (find(input_obtained.begin(), input_obtained.end(), inpId) == input_obtained.end())
                {
                    fit = false;
                    break;
                }
            }
            if (!fit)
            {
                break;
            }
        }
        if (fit)
        {
            result.push_back(i);
        }
    }
    return result;
}

vector<GPFunctionTree*> computePoint::outputs()
{
    vector<GPFunctionTree*> result;
    const vector<const GPProducerUtils::func*>& data = getData();
    for (int i=0; i<data.size(); ++i)
    {
        GPFunctionTree* p = new GPFunctionTree(data[i]->basic);
        result.push_back(p);
    }
    for (auto child : mChild)
    {
        computePoint* p = (computePoint*)(child.get());
        GPASSERT(p->mParent < result.size());
        GPFunctionTree* parent = result[p->mParent];
        const GPProducerUtils::func* pf = data[p->mParent];
        int cur = 0;
        auto childrens = p->outputs();
        for (int i=0; i<pf->inputs.size(); ++i)
        {
            if (pf->useChildrenInput[i] > 0)
            {
                GPASSERT(cur<childrens.size());
                parent->addPoint(childrens[cur++]);
            }
            else
            {
                parent->addPoint(new GPFunctionTree((size_t)0));
            }
        }
    }
    return result;
}
vector<const GPProducerUtils::func*> computePoint::getDependOutput()
{
    vector<const GPProducerUtils::func*> result;
    computePoint* cur = (computePoint*)(mDepend);
    computePoint* self = this;
    while(NULL!=cur)
    {
        const vector<const GPProducerUtils::func*>& data = cur->getData();
        result.push_back(data[self->mParent]);
        self = cur;
        cur = (computePoint*)(cur->mDepend);
    }
    return result;
}
bool computePoint::vGrow()
{
    bool success = true;
    const vector<const GPProducerUtils::func*>& data = getData();
    vector<const GPProducerUtils::func*> currentOutputId = getDependOutput();
    mChild.clear();
    for (int i=0; i<data.size(); ++i)
    {
        const GPProducerUtils::func* f = data[i];
        const vector<vector<const GPProducerUtils::func*> >& inputData = f->tables;
        currentOutputId.push_back(data[i]);
        vector<int> avail = filter(inputData, currentOutputId);
        currentOutputId.erase(currentOutputId.end()-1);
        if (!avail.empty())
        {
            computePoint* midres = new computePoint(inputData, avail, mInput);
            midres->mDepend = this;
            midres->mParent = i;
            mChild.push_back(midres);
        }
        else if (!inputData.empty())
        {
            //This point is invalid, inform it
            success = false;
            break;
        }
    }
    return success;
}
vector<GPFunctionTree*> computeSearchTree::output()
{
    return ((computePoint*)(mRoot.get()))->outputs();
}
bool computeSearchTree::readyToOutput()
{
    return true;
}
