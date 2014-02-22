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
#include "system/GenerateSystem.h"
#include <list>
#include <stdlib.h>
#include "utils/debug.h"
#include "system/recurse_tree.h"
#include "core/AbstractGP.h"
using namespace std;

#define LIMIT_SIZE 100

computeFunction GenerateSystem::vGetCompute(int id)
{
    assert(NULL!=mComputeSystem);
    return mComputeSystem->getFunction(id);
}


vector<int> GenerateSystem::searchSequence(int out)
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
    if (NULL!=mComputeSystem)
    {
        for (int i=0; i<result.size()/3; ++i)
        {
            result[i*3+1] = allocSet(mComputeSystem->getStatusId(result[i*3]));
        }
    }
    return result;
}


vector<int> GenerateSystem::searchRandSequence(int outputFunctionId)
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
                inputFuncs = this->searchSequence(functionId);
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
            AbstractGP::loadUnitFunction(result, functionId, allocSet(mComputeSystem->getStatusId(functionId)), n);
        }
        else
        {
            AbstractGP::loadUnitFunction(result, functionId, -1, n);
        }
        cacheQueue.erase(cacheQueue.begin());
    }
    return result;
}


void GenerateSystem::setComputeSystem(computeSystem* comsys)
{
    mComputeSystem = comsys;
    vector<int> output = mComputeSystem->getOutputFunctions();
    if (!output.empty())
    {
        mDefaultOutput = output[0];
    }
}

int GenerateSystem::searchType(const std::string& type)
{
    return -1;
}
