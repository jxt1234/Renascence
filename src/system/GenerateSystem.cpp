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
#include <algorithm>
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

IGPAutoDefFunction* GenerateSystem::vCreateFunctionFromName(const std::string& name)
{
    assert(NULL!=mComputeSystem);
    int id = mComputeSystem->vQueryFuncId(name);
    const computeSystem::function& f = mComputeSystem->getDetailFunction(id);
    class simpleADF:public IGPAutoDefFunction
    {
        public:
            simpleADF(computeFunction f, int i, int o):mF(f), mInp(i), mOut(o){}
            ~simpleADF(){}

            virtual GP_Output run(const GP_Input& input)
            {
                return mF(input);
            }

            virtual int inputNumber() const {return mInp;}
            virtual int outputNumber() const {return mOut;}
        private:
            computeFunction mF;
            int mInp;
            int mOut;
    };
    IGPAutoDefFunction* result = new simpleADF(f.basic, f.inputType.size(), f.outputType.size());
    return result;
}

int GenerateSystem::vQueryInputsNumber(int id)
{
    assert(NULL!=mComputeSystem);
    const computeSystem::function& f = mComputeSystem->getDetailFunction(id);
    return f.inputType.size();
}

int GenerateSystem::vQueryOutputNumber(int id)
{
    assert(NULL!=mComputeSystem);
    const computeSystem::function& f = mComputeSystem->getDetailFunction(id);
    return f.outputType.size();
}

vector<int> GenerateSystem::searchSequence(int out)
{
    //Generate appointed output
    vector<vector<int> > warpOutput;
    warpOutput.clear();
    vector<int> output_vector(1, out);
    warpOutput.push_back(output_vector);
    vector<int> avail(1,0);
    //Generate approciate recurse_vector
    computePoint* start = new computePoint(warpOutput, avail, mComputeSystem);
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

std::vector<std::vector<int> > GenerateSystem::searchAllSequence(int output)
{
    vector<vector<int> > warpOutput;
    vector<int> output_vector(1, output);
    warpOutput.push_back(output_vector);
    vector<int> avail(1,0);
    computePoint* start = new computePoint(warpOutput, avail, mComputeSystem);
    computeSearchTree tree(start);
    vector<vector<int> > queues = tree.searchAll();
    if (NULL!=mComputeSystem)
    {
        for (int v=0; v<queues.size(); ++v)
        {
            vector<int>& result = queues[v];
            for (int i=0; i<result.size()/3; ++i)
            {
                result[i*3+1] = allocSet(mComputeSystem->getStatusId(result[i*3]));
            }
        }
    }
    return queues;
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
}

vector<int> GenerateSystem::searchType(const std::string& type)
{
    assert(NULL!=mComputeSystem);
    int typeId = queryType(type);
    return mComputeSystem->getOutputFunctions(typeId);
}

/*FIXME Currently, we assume random be false and inputRepeat be true, just return the first short tree by algorithm*/
IGPAutoDefFunction* GenerateSystem::vCreateFunction(const std::vector<int>& outputType, const std::vector<int>& inputType, bool inputRepeat, bool random)
{
    assert(NULL!=mComputeSystem);
    /*TODO if mComputeSystem's inputType and outputType is the same, return the cached one*/
    mComputeSystem->mOutputTypeId = outputType;
    mComputeSystem->mInputTypeId = inputType;
    AbstractGP* gp = NULL;
    IGPAutoDefFunction* res = NULL;
    /*Find all available output function*/
    vector<vector<int> > warpOutput;
    _findMatchedFuncton(warpOutput, outputType);
    vector<int> avail(1,warpOutput.size()-1);
    /*Get All sequence*/
    computePoint* start = new computePoint(warpOutput, avail, mComputeSystem);
    computeSearchTree tree(start);
    /*TODO random for result*/
    vector<int> queue = tree.searchOne();
    //TODO Allow queue.empty()
    assert(!queue.empty());
    //if (result.empty()) return NULL;
    _allocStatusForQueue(queue);
    gp = new AbstractGP;
    initGP(gp, queue);
    res = this->vCreateADFFromGP(gp);
    gp->decRef();
    return res;
}

void GenerateSystem::_allocStatusForQueue(std::vector<int>& queue)
{
    /*Alloc Status*/
    for (int i=0; i<queue.size()/3; ++i)
    {
        queue[i*3+1] = allocSet(mComputeSystem->getStatusId(queue[i*3]));
    }
}

std::vector<IGPAutoDefFunction*> GenerateSystem::vCreateAllFunction(const std::vector<int>& outputType, const std::vector<int>& inputType, bool inputRepeat)
{
    assert(NULL!=mComputeSystem);
    /*TODO if mComputeSystem's inputType and outputType is the same, return the cached one*/
    mComputeSystem->mOutputTypeId = outputType;
    mComputeSystem->mInputTypeId = inputType;
    AbstractGP* gp = NULL;
    /*Find all available output function*/
    vector<vector<int> > warpOutput;
    _findMatchedFuncton(warpOutput, outputType);
    vector<int> avail;
    for (int i=0; i<warpOutput.size(); ++i)
    {
        avail.push_back(i);
    }
    /*Get All sequence*/
    computePoint* start = new computePoint(warpOutput, avail, mComputeSystem);
    computeSearchTree tree(start);
    vector<vector<int> > queue = tree.searchAll();
    vector<IGPAutoDefFunction*> res;
    for (int i=0; i<queue.size(); ++i)
    {
        _allocStatusForQueue(queue[i]);
        gp = new AbstractGP;
        initGP(gp, queue[i]);
        IGPAutoDefFunction* f = this->vCreateADFFromGP(gp);
        gp->decRef();
        res.push_back(f);
    }
    return res;
}


void GenerateSystem::_findMatchedFuncton(std::vector<std::vector<int> >& warpOutput, const std::vector<int>& outputType)
{
    for (int i=0; i < mComputeSystem->getFunctionNumber(); ++i)
    {
        const computeSystem::function& f = mComputeSystem->getDetailFunction(i);
        const vector<int>& out = f.outputType;
        bool match = true;
        for (int j=0; j<outputType.size(); ++j)
        {
            if (find(out.begin(), out.end(), outputType[j]) == out.end())
            {
                match = false;
                break;
            }
        }
        if (match)
        {
            vector<int> output;
            output.push_back(i);
            warpOutput.push_back(output);
        }
    }
    assert(!warpOutput.empty());
    //if (warpOutput.empty()) return NULL;
}
bool GenerateSystem::initGP(AbstractGP* tree, const std::vector<int>& queue)
{
    assert(NULL!=tree);
    int cur = 0;
    tree->replacePoint(queue, cur);
    this->vSetInputNumber(tree);
    return true;
}

void GenerateSystem::freeStatus(AbstractGP* tree)
{
    vector<int> status = tree->getStatus();
    for (int i=0; i<status.size(); ++i)
    {
        freeSet(status[i]);
    }
}
bool GenerateSystem::initGP(AbstractGP* tree, int output, bool random)
{
    assert(NULL!=tree);
    vector<int> queue;
    if (random)
    {
        queue = this->searchRandSequence(output);
    }
    else
    {
        queue = this->searchSequence(output);
    }
    if (queue.empty()) return false;
    /*Replace*/
    vector<int> status = tree->getStatus();
    for (int i=0; i<status.size(); ++i)
    {
        freeSet(status[i]);
    }
    int cur = 0;
    tree->replacePoint(queue, cur);
    return true;
}


bool GenerateSystem::initGP(AbstractGP* tree, const std::string& output, bool random)
{
    vector<int> out = this->searchType(output);
    if (out.empty()) return false;
    if (random)
    {
        int outputFunc = rand()%out.size();
        return initGP(tree, outputFunc, true);
    }
    /*TODO For random scene, wide search a available sequence*/
    for (int i=0; i<out.size(); ++i)
    {
        bool res = initGP(tree, out[i], random);
        if (res)
        {
            return true;
        }
    }
    return false;
}

void GenerateSystem::mutate(AbstractGP* tree)
{
    assert(NULL!=tree);
    int out = tree->funcId();
    initGP(tree, out, true);
}
