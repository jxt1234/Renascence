#include "system/GenerateSystem.h"
#include <list>
#include <stdlib.h>
#include "utils/debug.h"
#include "system/recurse_tree.h"
using namespace std;

#define LIMIT_SIZE 100

vector<int> GenerateSystem::getRandSequence()
{
    vector<int> result;
    if (NULL==mComputeSystem)
    {
        return result;
    }
    const vector<int> outputs = mComputeSystem->getOutputFunctions();
    int rootId = outputs[rand()%outputs.size()];
    return getRandSequenceWithOutput(rootId);
}

vector<int> GenerateSystem::searchOneWithOutput(int out)
{
    //Generate appointed output
    computePoint::setComputeSystem(mComputeSystem);
    vector<vector<int> > warpOutput;
    warpOutput.clear();
    vector<int> output_vector(1, out);
    warpOutput.push_back(output_vector);
    vector<int> avail(1,0);
    //Generate approciate recurse_vector
    computePoint* start = new computePoint(warpOutput, avail);
    computeSearchTree tree(start);
    vector<int> result = tree.searchOne();
    if (!mComputeSystem)
    {
        for (int i=0; i<result.size()/3; ++i)
        {
            result[i*3+1] = mComputeSystem->allocateStatus(result[i*3]);
        }
    }
}

vector<int> GenerateSystem::searchOneSequence()
{
    vector<int> result;
    if (NULL==mComputeSystem)
    {
        return result;
    }
    const vector<int>& outputs = mComputeSystem->getOutputFunctions();
    for (int i=0; i<outputs.size(); ++i)
    {
        result = searchOneWithOutput(outputs[i]);
        if (!result.empty())
        {
            break;
        }
    }
    return result;
}

vector<int> GenerateSystem::getRandSequenceWithOutput(int outputFunctionId)
{
    vector<int> result;
    list<int> cacheQueue;
    if (NULL==mComputeSystem)
    {
        return result;
    }
    cacheQueue.push_back(outputFunctionId);
    while (!cacheQueue.empty())
    {
        int functionId = *(cacheQueue.begin());
        const vector<vector<int> >& inputFuncPairs = mComputeSystem->getAvailableFunctionInputs(functionId);
        int n = 0;
        if (!inputFuncPairs.empty())
        {
            //Enlong the chain
            vector<int> inputFuncs;
            //Lenght Limit
            if (LIMIT_SIZE < result.size())
            {
                inputFuncs = searchOneWithOutput(functionId);
                for (int i=0; i<inputFuncs.size(); ++i)
                {
                    result.push_back(inputFuncs[i]);
                }
                cacheQueue.erase(cacheQueue.begin());
                continue;
            }
            else
            {
                inputFuncs = inputFuncPairs[rand()%inputFuncPairs.size()];
            }
            n = inputFuncs.size();
            for (int i=0; i<inputFuncs.size(); ++i)
            {
                cacheQueue.push_back(inputFuncs[i]);
            }
        }
        //Input information to result
        if (mComputeSystem)
        {
            GeneticProgram::loadUnitFunction(result, functionId, mComputeSystem->allocateStatus(functionId), n);
        }
        else
        {
            GeneticProgram::loadUnitFunction(result, functionId, -1, n);
        }
        cacheQueue.erase(cacheQueue.begin());
    }
    return result;
}


