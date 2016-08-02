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
#include "MGPExecutor.h"
#include "MGPUtils.h"
#include "AutoStorage.h"
#include "math/GPCarryVaryGroup.h"
#include <string.h>
#include "MGPKeyMatcher.h"
#include "MGPThread.h"
#include "GPClock.h"

static GPContents* _loadContent(int inputNumber, GPPieces** inputs, unsigned int* inputKeys)
{
    GPContents* totalInput = new GPContents;
    int pos = 0;
    for (int i=0; i<inputNumber; ++i)
    {
        GPContents* current = inputs[i]->vLoad(inputKeys+pos, inputs[i]->nKeyNumber);
        totalInput->merge(*current);
        current->decRef();
        pos+= inputs[i]->nKeyNumber;
    }
    return totalInput;
}


class MGPExecutor::ThreadData
{
public:
    ThreadData(const IGPFunctionContext* context, const std::string& formula)
    {
        mFunction = context->vCreateContentFunction(formula, "", std::vector<const IStatusType*>());//TODO
    }

    ~ThreadData()
    {
        delete mFunction;
    }

    IGPFunction* get() const
    {
        return mFunction;
    }

private:
    IGPFunction* mFunction;
};

MGPExecutor::MGPExecutor(const IGPFunctionContext* context, const std::string& formula, const std::string& condition, const std::string& variable, int threadNum, IParallelMachine::PARALLELTYPE type, const GPParallelType::KEYS& outputKeys, const GPParallelType::KEYS& variableKey)
{
    MGPASSERT(NULL!=context);
    MGPASSERT(threadNum>1);
    MGPASSERT(type == IParallelMachine::MAP || type == IParallelMachine::REDUCE);
    mMainData = new ThreadData(context, formula);
    for (int i=0; i<threadNum; ++i)
    {
        mUserData.push_back(new ThreadData(context, formula));
    }

    std::vector<void*> userdata;
    for (auto t : mUserData)
    {
        userdata.push_back(t);
    }
    mPool = new MGPThreadPool(userdata);
    mType = type;
    mOutputKey = outputKeys;
    if (!condition.empty())
    {
        mCondition = context->vCreateFloatFunction(condition, variable);
    }
    mVariableKey = variableKey;
}

MGPExecutor::~MGPExecutor()
{
    delete mPool;
    for (auto t : mUserData)
    {
        delete t;
    }
    delete mMainData;
}

class MapRunnable:public MGPThreadPool::Runnable
{
public:
    MapRunnable(GPPieces** input, unsigned int inputNum, unsigned int* inputKeys, int inputKeyNum, GPPieces* output, unsigned int* outputKeys, unsigned int outputKeyNum, const GPParallelType::KEYS& key, MGPMutex& inputMutex, MGPMutex& outputMutex):mVariableKey(key), mInputMutex(inputMutex), mOutputMutex(outputMutex)
    {
        MGPASSERT(inputKeyNum>0);
        MGPASSERT(outputKeyNum>0);
        mInput = input;
        mInputNum = inputNum;
        mOutput = output;
        mInputKeys = inputKeys;
        mOutputKeys = outputKeys;
    }
    virtual ~MapRunnable()
    {
    }
    virtual void vRun(void* threadData) override
    {
        //GPCLOCK;
        MGPExecutor::ThreadData* data = (MGPExecutor::ThreadData*)threadData;
        auto function = data->get();
        GPContents* orderedInput = NULL;
        GPContents* orderedOriginInput = NULL;
        {
            //GPCLOCK;
            MGPAutoMutex __m(mInputMutex);
            GPContents* totalInput = _loadContent(mInputNum, mInput, mInputKeys);
            orderedOriginInput = new GPContents;
            for (int i=0; i<mVariableKey.size(); ++i)
            {
                orderedOriginInput->pushContent(totalInput->getContent(mVariableKey[i].second));
            }
            totalInput->decRef();
            orderedInput = orderedOriginInput->copyAsNoOwner();
        }
        GPContents* output = NULL;
        {
            //GPCLOCK;
            output = function->vRun(orderedInput);
        }
        {
            //GPCLOCK;
            MGPAutoMutex __m(mInputMutex);
            orderedInput->decRef();
            orderedOriginInput->decRef();
        }
        {
            //GPCLOCK;
            MGPAutoMutex __m(mOutputMutex);
            mOutput->vSave(mOutputKeys, mOutput->nKeyNumber, output);
            output->decRef();
        }
    }
private:
    GPPieces** mInput;
    unsigned int mInputNum;
    
    unsigned int* mInputKeys;
    
    GPPieces* mOutput;
    unsigned int* mOutputKeys;
    const GPParallelType::KEYS& mVariableKey;
    
    MGPMutex& mInputMutex;
    MGPMutex& mOutputMutex;
};


bool MGPExecutor::vRun(GPPieces* output, GPPieces** inputs, int inputNumber) const
{
    if (IParallelMachine::MAP == mType)
    {
        return _mapRun(output, inputs, inputNumber);
    }
    return _reduceRun(output, inputs, inputNumber);
}
bool MGPExecutor::_mapRun(GPPieces* output, GPPieces** inputs, int inputNumber) const
{
    MGPASSERT(NULL!=output);
    MGPASSERT(NULL!=inputs);
    MGPASSERT(inputNumber>0);
    
    MGPKeyMatcher matcher(inputs, inputNumber, output, mOutputKey, mCondition.get());
    auto keymaps = matcher.get();

    MGPMutex inputMutex;
    MGPMutex outputMutex;
    
    int pos = 0;
    auto threadNum = mPool->getThreadNumber();
    std::vector<std::vector<MGPThreadPool::Runnable*>> runnableList;
    for (int i=0; i<threadNum; ++i)
    {
        std::vector<MGPThreadPool::Runnable*> r;
        runnableList.push_back(r);
    }

    for (auto& k : keymaps)
    {
        MapRunnable* runnable = new MapRunnable(inputs, inputNumber, k.second[0]->getKey(), k.second[0]->getKeyNumber(), output, k.first->getKey(), k.first->getKeyNumber(), mVariableKey, inputMutex, outputMutex);
        runnableList[pos%threadNum].push_back(runnable);
        pos++;
    }

    std::vector<GPPtr<MGPSema>> waitSemas;
    for (int i=1; i<threadNum; ++i)
    {
        //FUNC_PRINT(runnableList[i].size());
        GPPtr<MGPSema> sema = mPool->pushTask(MGPThreadPool::mergeRunnables(runnableList[i]));
        MGPASSERT(NULL!=sema.get());
        waitSemas.push_back(sema);
    }
    auto main_runnable = MGPThreadPool::mergeRunnables(runnableList[0]);
    main_runnable->vRun(mMainData);
    main_runnable->decRef();
    for (auto s : waitSemas)
    {
        //GPCLOCK;
        s->wait();
    }
    return true;
}

class Collector
{
public:
    Collector(GPContents* content)
    {
        mContents = content;
    }

    ~Collector()
    {
        if (NULL!=mContents)
        {
            mContents->decRef();
        }
    }
    GPContents* lock()
    {
        return mContents;
    }
    void unlock(GPContents* newContent)
    {
        if(newContent != mContents)
        {
            mContents->decRef();
            mContents = newContent;
        }
    }
private:
    GPContents* mContents;
};


class ReduceRunnable:public MGPThreadPool::Runnable
{
public:
    ReduceRunnable(Collector* collector, GPPieces** inputs, unsigned int inputNum, unsigned int* inputKeys, const GPParallelType::KEYS& keys, MGPMutex& inputMutex):mVariableKey(keys), mInputMutex(inputMutex)
    {
        mInput = inputs;
        mInputNum = inputNum;
        mInputKeys = inputKeys;
        mCollector = collector;
    }
    
    virtual ~ReduceRunnable()
    {
    }
    
    
    virtual void vRun(void* threadData) override
    {
        MGPExecutor::ThreadData* data = (MGPExecutor::ThreadData*)threadData;
        auto function = data->get();
        GPContents* oldContent = mCollector->lock();
        GPContents* input = NULL;
        GPContents* mergeInput = NULL;
        {
            MGPAutoMutex __m(mInputMutex);
            auto tempMergeInput = new GPContents;
            input = _loadContent(mInputNum, mInput, mInputKeys);
            for (int i=0; i<mVariableKey.size(); ++i)
            {
                auto v = mVariableKey[i];
                MGPASSERT(v.first<=1);
                if (v.first == 1)
                {
                    tempMergeInput->pushContent(oldContent->getContent(v.second));
                }
                else
                {
                    tempMergeInput->pushContent(input->getContent(v.second));
                }
            }
            mergeInput = tempMergeInput->copyAsNoOwner();
            tempMergeInput->decRef();
        }
        GPContents* tempOutput = function->vRun(mergeInput);
        {
            MGPAutoMutex __m(mInputMutex);
            mergeInput->decRef();
            input->decRef();
        }
        {
            oldContent->setContent(0, tempOutput->getContent(0));
            tempOutput->decRef();
            mCollector->unlock(oldContent);
        }
    }
private:
    Collector* mCollector;
    GPPieces** mInput;
    unsigned int mInputNum;
    unsigned int* mInputKeys;
    const GPParallelType::KEYS& mVariableKey;
    MGPMutex& mInputMutex;
};


class CollectRunnable:public MGPThreadPool::Runnable
{
public:
    CollectRunnable(const std::vector<Collector*>& collectors, const GPParallelType::KEYS& keys):mCollectors(collectors), mVariableKey(keys){}
    virtual ~CollectRunnable(){}
    virtual void vRun(void* threadData) override
    {
        MGPExecutor::ThreadData* data = (MGPExecutor::ThreadData*)threadData;
        auto function = data->get();
        MGPASSERT(!mCollectors.empty());
        GPContents* target = mCollectors[0]->lock();
        for (int i=1; i<mCollectors.size(); ++i)
        {
            GPContents* input = mCollectors[i]->lock();
            GPContents mergeInput;
            for (int j=0; j<mVariableKey.size(); ++j)
            {
                auto v = mVariableKey[j];
                MGPASSERT(v.first<=1);
                if (v.first == 1)
                {
                    mergeInput.pushContent(target->getContent(v.second));
                }
                else
                {
                    mergeInput.pushContent(input->getContent(v.second));
                }
            }
            mCollectors[i]->unlock(NULL);
            GPPtr<GPContents> tempOutput = function->vRun(&mergeInput);
            target->setContent(0, tempOutput->getContent(0));
        }
        mCollectors[0]->unlock(target);
    }
private:
    const std::vector<Collector*>& mCollectors;
    const GPParallelType::KEYS& mVariableKey;
};



bool MGPExecutor::_reduceRun(GPPieces* output, GPPieces** inputs, int inputNumber) const
{
    /*Collect Keys*/
    MGPKeyMatcher matcher(inputs, inputNumber, output, mOutputKey, mCondition.get());
    auto keymaps = matcher.get();
    size_t threadNumber = mPool->getThreadNumber();
    std::map<MGPKeyMatcher::Key*, std::vector<Collector*>> collectorMaps;
    std::vector<GPPtr<MGPSema>> waitSemas;

    MGPMutex inputMutex;
    MGPMutex outputMutex;
    for (auto& k : keymaps)
    {
        auto& outputKey = k.first;
        auto& inputKeys = k.second;
        /*Generate Collectors*/
        collectorMaps.insert(std::make_pair(outputKey, std::vector<Collector*>()));
        std::vector<Collector*>& collectors = collectorMaps.find(outputKey)->second;
        
        auto eachCollectSize = threadNumber;
        if (inputKeys.size() < threadNumber)
        {
            eachCollectSize = 1;
        }
        for (int i=0; i<eachCollectSize; ++i)
        {
            GPContents* target = _loadContent(inputNumber, inputs, inputKeys[i]->getKey());
            collectors.push_back(new Collector(target));
        }
        std::vector<std::vector<MGPThreadPool::Runnable*>> runnableList;
        for (int i=0; i<collectors.size(); ++i)
        {
            std::vector<MGPThreadPool::Runnable*> rr;
            runnableList.push_back(rr);
        }
        
        /*For Remain outputKey, Parralelly to reduce*/
        for (size_t i=eachCollectSize; i<inputKeys.size(); ++i)
        {
            size_t index = i % collectors.size();
            runnableList[index].push_back(new ReduceRunnable(collectors[index], inputs, inputNumber, inputKeys[i]->getKey(), mVariableKey, inputMutex));
        }
        for (int i=0; i<collectors.size(); ++i)
        {
            MGPThreadPool::Runnable* merge = MGPThreadPool::mergeRunnables(runnableList[i]);
            waitSemas.push_back(mPool->pushTask(merge));
        }
    }
    for (auto s : waitSemas)
    {
        s->wait();
    }
    waitSemas.clear();

    /*Collect All Collector*/
    for (auto& kv : collectorMaps)
    {
        auto& collectors = kv.second;
        if (collectors.size()>1)
        {
            waitSemas.push_back(mPool->pushTask(new CollectRunnable(collectors, mVariableKey)));
        }
    }
    for (auto s : waitSemas)
    {
        s->wait();
    }
    waitSemas.clear();

    /*Write to output*/
    for (auto& kv : collectorMaps)
    {
        GPContents* result = kv.second[0]->lock();
        output->vSave(kv.first->getKey(), kv.first->getKeyNumber(), result);
        kv.second[0]->unlock(NULL);
        for (auto c : kv.second)
        {
            delete c;
        }
    }
    return true;
}

