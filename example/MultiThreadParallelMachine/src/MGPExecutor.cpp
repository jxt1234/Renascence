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
#include "GPCarryVaryGroup.h"
#include <string.h>
#include "MGPKeyMatcher.h"
#include "MGPThread.h"

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
    MGPThreadPool* mPieceHandlerPool;
};

MGPExecutor::MGPExecutor(const IGPFunctionContext* context, const std::string& formula, const std::string& condition, int threadNum, IParallelMachine::PARALLELTYPE type, const std::vector<std::pair<unsigned int, unsigned int>>& outputKeys)
{
    MGPASSERT(NULL!=context);
    MGPASSERT(threadNum>1);
    MGPASSERT(type == IParallelMachine::MAP || type == IParallelMachine::REDUCE);
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
    mLoadPool = new MGPThreadPool(std::vector<void*>{NULL});
    mType = type;
    mOutputKey = outputKeys;
}

MGPExecutor::~MGPExecutor()
{
    delete mPool;
    delete mLoadPool;
    for (auto t : mUserData)
    {
        delete t;
    }
}
class FreeRunnalbe:public MGPThreadPool::Runnable
{
public:
    FreeRunnalbe(GPContents* target)
    {
        mTarget = target;
    }
    virtual ~FreeRunnalbe(){}
    virtual void vRun(void* threadData) override
    {
        mTarget->decRef();
    }

private:
    GPContents* mTarget;
};

class SaveRunnable:public MGPThreadPool::Runnable
{
public:
    SaveRunnable(unsigned int* outputKeys, GPPieces* output, GPContents* target)
    {
        mOutputKeys = outputKeys;
        mOutput = output;
        mTarget = target;
    }
    virtual ~SaveRunnable()
    {
        
    }
    virtual void vRun(void* threadData) override
    {
        mOutput->vSave(mOutputKeys, mOutput->nKeyNumber, mTarget);
        mTarget->decRef();
    }
private:
    GPContents* mTarget;
    GPPieces* mOutput;
    unsigned int* mOutputKeys;
};


class LoadRunnable:public MGPThreadPool::Runnable
{
public:
    LoadRunnable(GPContents** target, GPPieces** inputs, unsigned int inputNum, unsigned int* inputKeys)
    {
        mTarget = target;
        mInputNum = inputNum;
        mInputKeys = inputKeys;
        mTarget = target;
        mInput = inputs;
    }

    virtual ~LoadRunnable()
    {
        
    }
    virtual void vRun(void* threadData) override
    {
        MGPASSERT(mInputNum>=1);
        GPContents* totalInput = new GPContents;
        int pos = 0;
        for (int i=0; i<mInputNum; ++i)
        {
            GPContents* current = mInput[i]->vLoad(mInputKeys+pos, mInput[i]->nKeyNumber);
            totalInput->merge(*current);
            current->decRef();
            pos+= mInput[i]->nKeyNumber;
        }
        *mTarget = totalInput;
    }

private:
    GPContents** mTarget;
    GPPieces** mInput;
    unsigned int mInputNum;
    
    unsigned int* mInputKeys;
    
};

class MapRunnable:public MGPThreadPool::Runnable
{
public:
    MapRunnable(MGPThreadPool* loadpool, GPPieces** input, unsigned int inputNum, unsigned int* inputKeys, int inputKeyNum, GPPieces* output, unsigned int* outputKeys, unsigned int outputKeyNum)
    {
        MGPASSERT(inputKeyNum>0);
        MGPASSERT(outputKeyNum>0);
        mLoadPool = loadpool;
        mInput = input;
        mInputNum = inputNum;
        mOutput = output;
        mInputKeys = new unsigned int[inputKeyNum];
        ::memcpy(mInputKeys, inputKeys, sizeof(unsigned int)*inputKeyNum);
        mOutputKeys = new unsigned int[outputKeyNum];
        ::memcpy(mOutputKeys, outputKeys, sizeof(unsigned int)*outputKeyNum);
    }
    virtual ~MapRunnable()
    {
        delete [] mInputKeys;
        delete [] mOutputKeys;
    }
    virtual void vRun(void* threadData) override
    {
        MGPExecutor::ThreadData* data = (MGPExecutor::ThreadData*)threadData;
        auto function = data->get();
        GPContents* totalInput = NULL;
        Runnable* loadRunnable = new LoadRunnable(&totalInput, mInput, mInputNum, mInputKeys);
        GPPtr<MGPSema> sema = mLoadPool->pushTask(loadRunnable);
        sema->wait();
        MGPASSERT(NULL!=totalInput);
        GPContents* output = function->vRun(totalInput);
        sema = mLoadPool->pushTask(new SaveRunnable(mOutputKeys, mOutput, output));
        sema->wait();
        sema = mLoadPool->pushTask(new FreeRunnalbe(totalInput));
        sema->wait();
    }
private:
    GPPieces** mInput;
    unsigned int mInputNum;
    
    unsigned int* mInputKeys;
    
    GPPieces* mOutput;
    unsigned int* mOutputKeys;
    
    MGPThreadPool* mLoadPool;
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
    const size_t semaMaxNumber = mPool->getThreadNumber()*2;
    std::vector<GPPtr<MGPSema>> waitSemas;

    for (auto& k : keymaps)
    {
        GPPtr<MGPSema> sema = mPool->pushTask(new MapRunnable(mLoadPool, inputs, inputNumber, k.second[0]->getKey(), k.second[0]->getKeyNumber(), output, k.first->getKey(), k.first->getKeyNumber()));
        MGPASSERT(NULL!=sema.get());
        waitSemas.push_back(sema);
        if (waitSemas.size() > semaMaxNumber)
        {
            for (auto s : waitSemas)
            {
                s->wait();
            }
            waitSemas.clear();
        }
    }
    for (auto s : waitSemas)
    {
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
        mMutex.lock();
        return mContents;
    }
    void unlock(GPContents* newContent)
    {
        MGPASSERT(newContent != mContents);
        mContents->decRef();
        mContents = newContent;
        mMutex.unlock();
    }
private:
    GPContents* mContents;
    MGPMutex mMutex;
};

class ReduceRunnable:public MGPThreadPool::Runnable
{
public:
    ReduceRunnable(MGPThreadPool* loadPool, Collector* collector, GPPieces** inputs, unsigned int inputNum, unsigned int* inputKeys)
    {
        mLoadPool = loadPool;
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
        Runnable* loadRunnable = new LoadRunnable(&input, mInput, mInputNum, mInputKeys);
        GPPtr<MGPSema> sema = mLoadPool->pushTask(loadRunnable);
        sema->wait();
        input->merge(*oldContent);
        auto newContent = function->vRun(input);
        input->decRef();
        mCollector->unlock(newContent);
    }
private:
    Collector* mCollector;
    MGPThreadPool* mLoadPool;
    GPPieces** mInput;
    unsigned int mInputNum;
    unsigned int* mInputKeys;
    
};


class CollectRunnable:public MGPThreadPool::Runnable
{
public:
    CollectRunnable(const std::vector<Collector*>& collectors):mCollectors(collectors){}
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
            target->merge(*input);
            mCollectors[i]->unlock(NULL);
            target = function->vRun(target);
            mCollectors[0]->unlock(target);
        }
    }
private:
    const std::vector<Collector*>& mCollectors;
};



bool MGPExecutor::_reduceRun(GPPieces* output, GPPieces** inputs, int inputNumber) const
{
    /*Collect Keys*/
    MGPKeyMatcher matcher(inputs, inputNumber, output, mOutputKey, mCondition.get());
    auto keymaps = matcher.get();
    size_t threadNumber = mPool->getThreadNumber();
    std::map<MGPKeyMatcher::Key*, std::vector<Collector*>> collectorMaps;
    
    const size_t semaMaxNumber = mPool->getThreadNumber()*2;
    std::vector<GPPtr<MGPSema>> waitSemas;

    for (auto& k : keymaps)
    {
        auto& outputKey = k.first;
        auto& inputKeys = k.second;
        /*Generate Collectors*/
        std::vector<Collector*> collectors;
        collectorMaps.insert(std::make_pair(outputKey, collectors));
        
        auto eachCollectSize = threadNumber;
        if (inputKeys.size() < threadNumber)
        {
            eachCollectSize = 1;
        }
        for (int i=0; i<eachCollectSize; ++i)
        {
            GPContents* target = NULL;
            MGPThreadPool::Runnable* loadRunnable = new LoadRunnable(&target, inputs, inputNumber, inputKeys[0]->getKey());
            GPPtr<MGPSema> sema = mLoadPool->pushTask(loadRunnable);
            sema->wait();
            collectors.push_back(new Collector(target));
        }
        /*For Remain outputKey, Parralelly to reduce*/
        for (size_t i=eachCollectSize; i<inputKeys.size(); ++i)
        {
            GPPtr<MGPSema> sema = mPool->pushTask(new ReduceRunnable(mLoadPool, collectors[i%collectors.size()], inputs, inputNumber, inputKeys[i]->getKey()));
            waitSemas.push_back(sema);
            if (waitSemas.size() > semaMaxNumber)
            {
                for (auto s : waitSemas)
                {
                    s->wait();
                }
                waitSemas.clear();
            }
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
        waitSemas.push_back(mPool->pushTask(new CollectRunnable(collectors)));
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

