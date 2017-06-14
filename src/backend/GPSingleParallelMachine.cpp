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
#include "math/GPSingleTree.h"
#include "math/GPCarryVaryGroup.h"
#include <string.h>

class SingleExecutor:public IParallelMachine::Executor
{
public:
    class Handle:public GPRefCount
    {
    public:
        virtual void vReset() {}
        /*Should Clear input*/
        virtual void vHandle(IGPFunction* function, GPPieces** output, GPContents* input, unsigned int* outputKey, unsigned int keyNumber, GPContents* p) const = 0;
    };
};

class ReduceHandle:public SingleExecutor::Handle
{
public:
    ReduceHandle(GPParallelType::KEYS keys, GPParallelType::KEYS initKeys)
    {
        mKey = keys;
        mInit = false;
        mInitKey = initKeys;
    }
    
    virtual ~ ReduceHandle()
    {
    }
    virtual void vReset() override
    {
        mInit = false;
    }

    virtual void vHandle(IGPFunction* function, GPPieces** output, GPContents* input, unsigned int* outputKey, unsigned int keyNumber, GPContents* p) const override
    {
        if (!mInit)
        {
            for (int i=0; i<mInitKey.size(); ++i)
            {
                auto k = mInitKey[i];
                GPASSERT(k.first == 0);
                output[i]->vSave(outputKey, keyNumber, input->getContent(k.second));
            }
            mInit = true;
            return;
        }
        GPContents mergeInput;
        for (int i=0; i<mKey.size(); ++i)
        {
            if(mKey[i].first == 0)
            {
                mergeInput.pushContent(input->getContent(mKey[i].second));
            }
            else if (mKey[i].first == 1)
            {
                mergeInput.pushContent(output[mKey[i].second]->vLoad(outputKey, keyNumber));
            }
            else if (mKey[i].first == -1)
            {
                mergeInput.pushContent(p->getContent(mKey[i].second));
            }
            else
            {
                GPASSERT(false);
            }
        }
        GPPtr<GPContents> currentGPOutput = function->vRun(&mergeInput);
        for (int i=0; i<currentGPOutput->size(); ++i)
        {
            output[i]->vSave(outputKey, keyNumber, currentGPOutput->getContent(i));
        }
    }
private:
    GPParallelType::KEYS mKey;
    GPParallelType::KEYS mInitKey;
    mutable bool mInit;
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

    virtual void vHandle(IGPFunction* function, GPPieces** output, GPContents* input, unsigned int* outputKey, unsigned int keyNumber, GPContents* p) const override
    {
        GPContents mergeInput;
        for (int i=0; i<mKey.size(); ++i)
        {
            if(mKey[i].first == 0)
            {
                mergeInput.pushContent(input->getContent(mKey[i].second));
            }
            else if (mKey[i].first == -1)
            {
                mergeInput.pushContent(p->getContent(mKey[i].second));
            }
            else
            {
                GPASSERT(false);
            }
        }
        GPPtr<GPContents> currentGPOutput = function->vRun(&mergeInput);
        for (int i=0; i<currentGPOutput->size(); ++i)
        {
            output[i]->vSave(outputKey, keyNumber, currentGPOutput->getContent(i));
        }
    }
private:
    GPParallelType::KEYS mKey;
};


IParallelMachine::Executor GPSingleParallelMachine::vPrepare(const GPParallelType* data, PARALLELTYPE type, const IGPFunctionContext* context) const
{
    GPASSERT(NULL!=data);
    GPASSERT(NULL!=context);
    GPPtr<SingleExecutor::Handle> handle;
    switch (type)
    {
        case IParallelMachine::MAP:
            handle = new MapHandle(data->sFuncInfo.variableKey);
            break;
        case IParallelMachine::REDUCE:
            handle = new ReduceHandle(data->sFuncInfo.variableKey, data->mReduceInitKey);
            break;
        default:
            GPASSERT(0);
            break;
    }
    GPPtr<IGPFunction> func = context->vCreateContentFunction(data->sFuncInfo.formula);
    
    //TODO
    //, data->sFuncInfo.parameter, data->sFuncInfo.inputs);
    GPPtr<GPKeyIteratorFactory> keyFactory = new GPKeyIteratorFactory(data);

    return [=](GPPieces** outputs, int outputNumber, GPPieces** inputs, int inputNumber, GPContents* paramters) {
        GPASSERT(NULL!=paramters);
        GPASSERT(NULL!=outputs);
        GPASSERT(NULL!=inputs);
        GPASSERT(inputNumber>0);
        GPASSERT(outputNumber>0);
        auto keyIterator = keyFactory->create(inputs, inputNumber, outputs[0]);
        auto size = keyIterator->vGetSize();
        AUTOSTORAGE(keyOutput, unsigned int, (int)size.second);
        AUTOSTORAGE(keyInput, unsigned int, (int)size.first);
        bool hasData = keyIterator->vRewind(keyInput, keyOutput);
        if (!hasData)
        {
            GPASSERT(0);//TODO
            return false;
        }
        handle->vReset();
        do
        {
            /*Get Current Input*/
            unsigned int pos = 0;
            GPPtr<GPContents> currentGPInputs = new GPContents;
            for (int i=0; i<inputNumber; ++i)
            {
                auto ci = inputs[i]->vLoad(keyInput+pos, (int)inputs[i]->getKeySize().size());
                pos += inputs[i]->getKeySize().size();
                currentGPInputs->pushContent(ci);
            }
            handle->vHandle(func.get(), outputs, currentGPInputs.get(), keyOutput, size.second, paramters);
        } while (keyIterator->vNext(keyInput, keyOutput));
        
        return true;
    };
}



GPPieces* GPSingleParallelMachine::vCreatePieces(const char* description, const IStatusType* types, unsigned int* keys, int keyNum, USAGE usage) const
{
    GPASSERT(keyNum <= 10);
    GPPieces* pieces = NULL;
    std::vector<unsigned int> keysV;
    for (int i=0; i<keyNum; ++i)
    {
        keysV.push_back(keys[i]);
    }
    switch (usage)
    {
        case IParallelMachine::CACHE:
        {
            pieces = GPPieceFactory::createMemoryPiece(keysV);
            break;
        }
        case IParallelMachine::INPUT:
        case IParallelMachine::OUTPUT:
        {
            pieces = GPPieceFactory::createLocalFilePiece(keysV, types, description, 0);
            GPASSERT(NULL!=pieces);
            break;
        }
        default:
            GPASSERT(0);
            break;
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
    auto readKey = readPieces->getKeySize();
    auto writeKey = writePieces->getKeySize();
    GPASSERT(readKey.size() == writeKey.size());
    auto n = (int)readKey.size();
    
    for (int i=0; i<readKey.size(); ++i)
    {
        GPASSERT(readKey[i] == writeKey[i]);
    }
    GPCarryVaryGroup group(readKey.data(), n);
    AUTOSTORAGE(keyCurrent, unsigned int, n);
    group.start(keyCurrent, n);
    do
    {
        auto ci = readPieces->vLoad(keyCurrent, n);
        GPASSERT(NULL!=ci.get());
        writePieces->vSave(keyCurrent, n, ci);
    } while (group.next(keyCurrent, n));
    return true;
}
