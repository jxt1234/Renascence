#include "system/recurse_tree.h"
#include <list>
#include "system/GenerateSystem.h"

computeSystem* computePoint::gSystem = NULL;
vector<int> computePoint::filter(const vector<vector<int> >& combo, vector<int> output)
{
    vector<int> result;
    for (int i=0; i<combo.size(); ++i)
    {
        const vector<int>& comboUnit = combo[i];
        bool fit = true;
        for (int j=0; j<comboUnit.size(); ++j)
        {
            for (int k=0; k<output.size(); ++k)
            {
                if (comboUnit[j] == output[k])
                {
                    fit = false;
                    break;
                }
            }
        }
        if (fit)
        {
            result.push_back(i);
        }
    }
    return result;
}


vector<int> computePoint::getDependOutput()
{
    vector<int> result;
    computePoint* cur = dynamic_cast<computePoint*>(mDepend);
    while(NULL!=cur)
    {
        const vector<int>& data = cur->getData();
        for (int i=0; i<data.size(); ++i)
        {
            result.push_back(data[i]);
        }
        cur = dynamic_cast<computePoint*>(cur->mDepend);
    }
    return result;
}
bool computePoint::vGrow()
{
    bool success = true;
    do
    {
        CHECK_POINTER(gSystem);
        const vector<int>& data = getData();
        vector<int> currentOutputId = getDependOutput();
        for (int i=0; i<data.size(); ++i)
        {
            const vector<vector<int> >& inputData = gSystem->getAvailableFunctionInputs(data[i]);
            currentOutputId.push_back(data[i]);
            vector<int> avail = filter(inputData, currentOutputId);
            currentOutputId.erase(currentOutputId.end()-1);
            if (!avail.empty())
            {
                carryPoint* res=new computePoint(inputData, avail);
                computePoint* midres = dynamic_cast<computePoint*>(res);
                midres->mDepend = this;
                midres->mParent = i;
                mChild.push_back(res);
            }
            else if (!inputData.empty())
            {
                //This point is invalid, inform it
                success = false;
            }
        }
    }while(false);
    return success;
}
vector<int> computeSearchTree::output()
{
    computePoint* cur;
    vector<int> result;
    list<carryPoint*> queue;
    queue.push_back(mRoot);
    while(!queue.empty())
    {
        cur = dynamic_cast<computePoint*>(*(queue.begin()));
        //Construct search queue
        for (int i=0; i<cur->mChild.size(); ++i)
        {
            queue.push_back(cur->mChild[i]);
        }
        const vector<int>& data = cur->getData();
        //Construct the inputsNumbers
        vector<int> inputNumber(data.size(),0);
        for (int i=0; i<cur->mChild.size(); ++i)
        {
            computePoint* child = dynamic_cast<computePoint*>(cur->mChild[i]);
            inputNumber[child->mParent] = child->getData().size();
        }
        for (int i=0; i<data.size(); ++i)
        {
            GeneticProgram::loadUnitFunction(result, data[i], 0,inputNumber[i]);
        }
        queue.erase(queue.begin());
    }
    return result;
}
bool computeSearchTree::readyToOutput()
{
    return true;
}
