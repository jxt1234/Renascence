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
#include "backend/GPKeyIteratorFactory.h"
#include "backend/GPSingleParallelMachine.h"
#include "core/GPPieceFactory.h"
#include "core/IGPAutoDefFunction.h"
#include "math/GPSingleTree.h"
#include "math/GPCarryVaryGroup.h"
#include <string.h>

class SingleExecutor:public IParallelMachine::Executor
{
public:
    class Handle:public GPRefCount
    {
    public:
        /*Should Clear input*/
        virtual void vHandle(IGPFunction* function, GPPieces* output, GPContents* input, unsigned int* outputKey, unsigned int keyNumber) const = 0;
    };
    
    SingleExecutor(GPPtr<IGPFunction> f, GPPtr<GPKeyIteratorFactory> c, const std::vector<std::pair<unsigned int, unsigned int>>& outputKey, GPPtr<Handle> h):mFunction(f), mKeyIteratorFactory(c), mOutputKey(outputKey), mHandle(h)
    {
    }
    
    virtual ~SingleExecutor()
    {
    }
    
    virtual bool vRun(GPPieces* output, GPPieces** inputs, int inputNumber) const
    {
        //GPCLOCK;
        GPASSERT(NULL!=output);
        GPASSERT(NULL!=inputs);
        GPASSERT(inputNumber>0);
        auto keyIterator = mKeyIteratorFactory->create(inputs, inputNumber, output);
        auto size = keyIterator->vGetSize();
        AUTOSTORAGE(keyOutput, unsigned int, (int)size.second);
        AUTOSTORAGE(keyInput, unsigned int, (int)size.first);
        bool hasData = keyIterator->vRewind(keyInput, keyOutput);
        if (!hasData)
        {
            GPASSERT(0);//TODO
            return false;
        }
        do
        {
//            for (int i=0; i<size.first; ++i)
//            {
//                printf("%d ", keyInput[i]);
//            }
//            printf(" -> ");
//            for (int i=0; i<size.second; ++i)
//            {
//                printf("%d ", keyOutput[i]);
//            }
//            printf("\n");
            
            /*Get Current Input*/
            unsigned int pos = 0;
            GPContents* currentGPInputs = new GPContents;
            for (int i=0; i<inputNumber; ++i)
            {
                GPContents* ci = inputs[i]->vLoad(keyInput+pos, inputs[i]->nKeyNumber);
                pos += inputs[i]->nKeyNumber;
                currentGPInputs->merge(*ci);
                ci->decRef();//Don't delete content
            }
            mHandle->vHandle(mFunction.get(), output, currentGPInputs, keyOutput, size.second);
            currentGPInputs->decRef();
        } while (keyIterator->vNext(keyInput, keyOutput));
        
        return true;
    }
private:
    GPPtr<IGPFunction> mFunction;
    GPPtr<GPKeyIteratorFactory> mKeyIteratorFactory;
    std::vector<std::pair<unsigned int, unsigned int>> mOutputKey;
    GPPtr<Handle> mHandle;
};

class MapHandle:public SingleExecutor::Handle
{
public:
    MapHandle(GPParallelType::KEYS keys)
    {
        mKey = keys;
    }
    
    virtual ~ MapHandle()
    {
    }

    virtual void vHandle(IGPFunction* function, GPPieces* output, GPContents* input, unsigned int* outputKey, unsigned int keyNumber) const override
    {
        GPContents mergeInput;
        for (int i=0; i<mKey.size(); ++i)
        {
            GPASSERT(mKey[i].first == 0);
            mergeInput.pushContent(input->getContent(mKey[i].second));
        }
        GPPtr<GPContents> currentGPOutput = function->vRun(&mergeInput);
        output->vSave(outputKey, keyNumber, currentGPOutput.get());
    }
private:
    GPParallelType::KEYS mKey;
};

class ReduceHandle:public SingleExecutor::Handle
{
public:
    ReduceHandle(GPParallelType::KEYS keys)
    {
        mKey = keys;
    }
    
    virtual ~ ReduceHandle()
    {
    }
    
    virtual void vHandle(IGPFunction* function, GPPieces* output, GPContents* input, unsigned int* outputKey, unsigned int keyNumber) const override
    {
        GPPtr<GPContents> oldOutput = output->vLoad(outputKey, keyNumber);
        if (NULL == oldOutput.get())
        {
            output->vSave(outputKey, keyNumber, input);
            return;
        }
        GPContents mergeOutput;
        for (int i=0; i<mKey.size(); ++i)
        {
            GPASSERT(mKey[i].first <=1);
            if (0 == mKey[i].first)
            {
                mergeOutput.pushContent(input->getContent(mKey[i].second));
            }
            else
            {
                mergeOutput.pushContent(oldOutput->getContent(mKey[i].second));
            }
        }
        /*Merge, the oldOutput should be before the input*/
        GPPtr<GPContents> currentGPOutput = function->vRun(&mergeOutput);
        output->vSave(outputKey, keyNumber, currentGPOutput.get());
    }
    GPParallelType::KEYS mKey;
};



IParallelMachine::Executor* GPSingleParallelMachine::vPrepare(const GPParallelType* data, PARALLELTYPE type) const
{
    GPASSERT(NULL!=data);
    GPASSERT(NULL!=data->pContext);
    GPPtr<SingleExecutor::Handle> handle;
    switch (type)
    {
        case IParallelMachine::MAP:
            handle = new MapHandle(data->sFuncInfo.variableKey);
            break;
        case IParallelMachine::REDUCE:
            handle = new ReduceHandle(data->sFuncInfo.variableKey);
            break;
        default:
            GPASSERT(0);
            break;
    }
    GPPtr<IGPFunction> func = data->pContext->vCreateContentFunction(data->sFuncInfo.formula, data->sFuncInfo.parameter, data->sFuncInfo.inputs);
    GPPtr<IKeyFunction> condition;
    if (!data->sConditionInfo.sConditionFormula.empty())
    {
        condition = data->pContext->vCreateKeyFunction(data->sConditionInfo.sConditionFormula, data->sVariableInfo);
    }
    GPPtr<GPKeyIteratorFactory> keyFactory = new GPKeyIteratorFactory(data);

    Executor* executor = new SingleExecutor(func, keyFactory, data->mOutputKey, handle);
    return executor;
}



GPPieces* GPSingleParallelMachine::vCreatePieces(const char* description, std::vector<const IStatusType*> types, unsigned int* keys, int keyNum, USAGE usage) const
{
    GPASSERT(keyNum <= 10);
    GPPieces* pieces = NULL;
    bool write = true;
    switch (usage) {
        case IParallelMachine::CACHE:
        {
            std::vector<unsigned int> keysV;
            for (int i=0; i<keyNum; ++i)
            {
                keysV.push_back(keys[i]);
            }
            pieces = GPPieceFactory::createMemoryPiece(keysV);
            break;
        }
        case IParallelMachine::INPUT:
            write = false;
        case IParallelMachine::OUTPUT:
        {
            pieces = GPPieceFactory::createLocalFilePiece(types, description, 0);
            GPASSERT(NULL!=pieces);
            break;
        }
        default:
            GPASSERT(0);
            break;
    }
    pieces->pTypes = types;
    pieces->nKeyNumber = keyNum;
    for (int i=0; i<keyNum; ++i)
    {
        pieces->pKeySize[i] = keys[i];
    }
    if (keyNum == 0 && usage != IParallelMachine::OUTPUT)
    {
        pieces->pKeySize[0] = 1;
        pieces->nKeyNumber = 1;
    }
    return pieces;
}

bool GPSingleParallelMachine::vCopyPieces(GPPieces* readPieces, GPPieces* writePieces) const
{
    /*TODO, Remove assert
     *When not match, return false
     */
    GPASSERT(NULL!=readPieces);
    GPASSERT(NULL!=writePieces);
    GPASSERT(readPieces->nKeyNumber == writePieces->nKeyNumber || writePieces->nKeyNumber == 0);
    if (0 == writePieces->nKeyNumber)
    {
        writePieces->nKeyNumber = readPieces->nKeyNumber;
        for (int i=0; i<readPieces->nKeyNumber; ++i)
        {
            writePieces->pKeySize[i] = readPieces->pKeySize[i];
        }
    }
    for (int i=0; i<readPieces->nKeyNumber; ++i)
    {
        GPASSERT(readPieces->pKeySize[i] == writePieces->pKeySize[i]);
    }
    GPCarryVaryGroup group(readPieces->pKeySize, readPieces->nKeyNumber);
    AUTOSTORAGE(keyCurrent, unsigned int, readPieces->nKeyNumber);
    group.start(keyCurrent, readPieces->nKeyNumber);
    do
    {
        GPContents* ci = readPieces->vLoad(keyCurrent, readPieces->nKeyNumber);
        GPASSERT(NULL!=ci);
        writePieces->vSave(keyCurrent, readPieces->nKeyNumber, ci);
        ci->decRef();
    } while (group.next(keyCurrent, readPieces->nKeyNumber));
    return true;
}
