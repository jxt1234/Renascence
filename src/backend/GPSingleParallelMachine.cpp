/******************************************************************
 Copyright 2016, Jiang Xiao-tang
 
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
#include "head.h"
#include "backend/GPSingleParallelMachine.h"
#include "core/GPPieceFactory.h"
#include "core/IGPAutoDefFunction.h"
#include "math/GPSingleTree.h"
#include "math/GPCarryVaryGroup.h"
#include <string.h>

class PieceInMemoryCreator:public IParallelMachine::Creator
{
public:
    PieceInMemoryCreator(const std::vector<std::pair<unsigned int, unsigned int>>& outputKey)
    {
        mOutputKey = outputKey;
    }
    
    virtual GPPieces* vPrepare(GPPieces** inputs, int inputNumber) const override
    {
        GPASSERT(NULL!=inputs);
        GPASSERT(inputNumber>0);
        std::vector<unsigned int> keyDms;
        for (auto& p : mOutputKey)
        {
            GPASSERT(p.first < inputNumber);//TODO
            GPPieces* inp = inputs[p.first];
            GPASSERT(inp->nKeyNumber > p.second);//TODO
            keyDms.push_back(inp->pKeySize[p.second]);
        }
        return GPPieceFactory::createMemoryPiece(keyDms);
    }
private:
    std::vector<std::pair<unsigned int, unsigned int>> mOutputKey;
};

class SingleExecutor:public IParallelMachine::Executor
{
public:
    class Handle:public GPRefCount
    {
    public:
        /*Should Clear input*/
        virtual void vHandle(IGPFunction* function, GPPieces* output, GPContents* input, unsigned int* outputKey, unsigned int keyNumber) const = 0;
    };
    
    SingleExecutor(GPPtr<IGPFunction> f, GPPtr<IGPFloatFunction> c, const std::vector<std::pair<unsigned int, unsigned int>>& outputKey, GPPtr<Handle> h):mFunction(f), mCondition(c), mOutputKey(outputKey), mHandle(h)
    {
    }
    
    virtual ~SingleExecutor()
    {
    }
    
    virtual bool vRun(GPPieces* output, GPPieces** inputs, int inputNumber) const
    {
        GPASSERT(NULL!=output);
        GPASSERT(NULL!=inputs);
        GPASSERT(inputNumber>0);
        /*Compute all dimesions*/
        unsigned int sumDim = 0;
        for (int i=0; i<inputNumber; ++i)
        {
            sumDim += inputs[i]->nKeyNumber;
        }
        AUTOSTORAGE(keyOutput, unsigned int, (int)mOutputKey.size());
        AUTOSTORAGE(keyOutputPos, unsigned int, (int)mOutputKey.size());
        for (int pos=0; pos < mOutputKey.size(); ++pos)
        {
            keyOutputPos[pos] = 0;
            auto p = mOutputKey[pos];
            for (int i=0; i<p.first; ++i)
            {
                keyOutputPos[pos] = keyOutputPos[pos] + inputs[i]->nKeyNumber;
            }
        }
        
        AUTOSTORAGE(keyDimesions, unsigned int, sumDim);
        AUTOSTORAGE(keyCurrent, unsigned int, sumDim);
        AUTOSTORAGE(keyCurrentFloat, GPFLOAT, sumDim);
        unsigned int pos = 0;
        for (int i=0; i<inputNumber; ++i)
        {
            ::memcpy(keyDimesions+pos, inputs[i]->pKeySize, sizeof(unsigned int)*inputs[i]->nKeyNumber);
            pos += inputs[i]->nKeyNumber;
        }
        
        GPCarryVaryGroup group(keyDimesions, sumDim);
        group.start(keyCurrent, sumDim);
        do
        {
            if (NULL!=mCondition.get())
            {
                for (int i=0; i<sumDim; ++i)
                {
                    keyCurrentFloat[i] = keyCurrent[i];
                }
                GPFLOAT c = mCondition->vRun(keyCurrentFloat, sumDim);
                if (c <= 0)
                {
                    continue;
                }
            }
            /*Compute Target Key*/
            for (int i=0; i<mOutputKey.size(); ++i)
            {
                keyOutput[i] = keyCurrent[keyOutputPos[i]];
            }
            
            /*Get Current Input*/
            pos = 0;
            GPContents* currentGPInputs = new GPContents;
            for (int i=0; i<inputNumber; ++i)
            {
                GPContents* ci = inputs[i]->vLoad(keyCurrent+pos, inputs[i]->nKeyNumber);
                pos += inputs[i]->nKeyNumber;
                currentGPInputs->merge(*ci);
                ci->decRef();//Don't delete content
            }
            mHandle->vHandle(mFunction.get(), output, currentGPInputs, keyOutput, (unsigned int)mOutputKey.size());
        } while (group.next(keyCurrent, sumDim));
        
        return true;
    }
private:
    GPPtr<IGPFunction> mFunction;
    GPPtr<IGPFloatFunction> mCondition;
    std::vector<std::pair<unsigned int, unsigned int>> mOutputKey;
    GPPtr<Handle> mHandle;
};

class MapHandle:public SingleExecutor::Handle
{
public:
    virtual void vHandle(IGPFunction* function, GPPieces* output, GPContents* input, unsigned int* outputKey, unsigned int keyNumber) const override
    {
        GPContents* currentGPOutput = function->vRun(input);
        output->vSave(outputKey, keyNumber, currentGPOutput);
        delete input;
    }
};

class ReduceHandle:public SingleExecutor::Handle
{
    virtual void vHandle(IGPFunction* function, GPPieces* output, GPContents* input, unsigned int* outputKey, unsigned int keyNumber) const override
    {
        GPContents* oldOutput = output->vLoad(outputKey, keyNumber);
        if (NULL == oldOutput)
        {
            output->vSave(outputKey, keyNumber, input);
            return;
        }
        /*Merge, the oldOutput should be before the input*/
        oldOutput->merge(*input);
        delete input;
        GPContents* currentGPOutput = function->vRun(oldOutput);
        output->vSave(outputKey, keyNumber, currentGPOutput);
    }
};



std::pair<IParallelMachine::Creator*, IParallelMachine::Executor*> GPSingleParallelMachine::vGenerate(const GPParallelType* data, PARALLELTYPE type) const
{
    GPASSERT(NULL!=data);
    GPASSERT(NULL!=data->pContext);
    Creator* creator = new PieceInMemoryCreator(data->mOutputKey);
    GPPtr<SingleExecutor::Handle> handle;
    switch (type)
    {
        case IParallelMachine::MAP:
            handle = new MapHandle;
            break;
        case IParallelMachine::REDUCE:
            handle = new ReduceHandle;
            break;
        default:
            GPASSERT(0);
    }
    GPPtr<IGPFunction> func = data->pContext->vCreateContentFunction(data->sFuncInfo.formula, data->sFuncInfo.parameter, data->sFuncInfo.inputs);
    GPPtr<IGPFloatFunction> condition;
    if (!data->sConditionInfo.sConditionFormula.empty())
    {
        condition = data->pContext->vCreateFloatFunction(data->sConditionInfo.sConditionFormula, data->sVariableInfo);
    }
    
    Executor* executor = new SingleExecutor(func, condition, data->mOutputKey, handle);
    return std::make_pair(creator, executor);
}

