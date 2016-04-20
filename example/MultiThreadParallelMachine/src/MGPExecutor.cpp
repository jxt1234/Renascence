#include "MGPExecutor.h"
#include "MGPUtils.h"
#include "AutoStorage.h"
#include "GPCarryVaryGroup.h"
#include <string.h>

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

class LoadRunnable:public MGPThreadPool::Runnable
{
public:
    LoadRunnable(GPContents** target, GPPieces** inputs, unsigned int inputNum, unsigned int* inputKeys)
    {
        mTarget = target;
        mInputNum = inputNum;
        mInputKeys = inputKeys;
        mTarget = target;
    }

    virtual ~LoadRunnable()
    {
        
    }
    virtual void vRun(void* threadData) override
    {
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
        sema = mLoadPool->pushTask(new FreeRunnalbe(totalInput));
        mOutput->vSave(mOutputKeys, mOutput->nKeyNumber, output);
        output->decRef();
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
    std::vector<GPPtr<MGPSema>> waitSemas;
    //TODO
    const size_t semaMaxNumber = mPool->getThreadNumber()*2;
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
        GPPtr<MGPSema> sema = mPool->pushTask(new MapRunnable(mLoadPool, inputs, inputNumber, keyCurrent, sumDim, output, keyOutput, (unsigned int)mOutputKey.size()));
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
    } while (group.next(keyCurrent, sumDim));
    return true;
}


bool MGPExecutor::_reduceRun(GPPieces* output, GPPieces** inputs, int inputNumber) const
{
    /*Collect Keys*/
    return true;
}

