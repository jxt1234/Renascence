#include "backend/GPBasicPiecesFunctionCreator.h"

class GPPiecesFunctionNode : public GPAbstractPoint
{
public:
    GPPiecesFunctionNode(GPPtr<IParallelMachine::Creator> creator, GPPtr<IParallelMachine::Executor> executor)
    {
        GPASSERT(NULL!=creator.get() && NULL!=executor.get());
        mCreator = creator;
        mExecutor = executor;
        mInputPos = -1;
    }
    
    GPPiecesFunctionNode(int inputPos)
    {
        GPASSERT(inputPos>=0);
        mInputPos = inputPos;
    }
    
    virtual ~GPPiecesFunctionNode()
    {
    }
    
    GPPieces* compute(GPPieces** inputs, int n)
    {
        if (mInputPos >= 0)
        {
            GPASSERT(mInputPos < n);
            return inputs[mInputPos];
        }
        int subNum = (int)mChildren.size();
        AUTOSTORAGE(subinputs, GPPieces*, subNum);
        AUTOSTORAGE(needClean, bool, subNum);
        for (int i=0; i<subNum; ++i)
        {
            GPPiecesFunctionNode* node = GPCONVERT(GPPiecesFunctionNode, mChildren[i]);
            subinputs[i] = node->compute(inputs, n);
            needClean[i] = false;
            if (node->mInputPos < 0)
            {
                needClean[i] = true;
            }
        }
        GPPieces* output = mCreator->vPrepare(subinputs, subNum);
        mExecutor->vRun(output, subinputs, subNum);
        for (int i=0; i<subNum; ++i)
        {
            if (needClean[i])
            {
                delete subinputs[i];
            }
        }
        return output;
    }

private:
    GPPtr<IParallelMachine::Creator> mCreator;
    GPPtr<IParallelMachine::Executor> mExecutor;
    int mInputPos;
};


class GPBasicPiecesFunction : public GPPiecesFunction
{
public:
    virtual GPPieces* vRun(GPPieces** inputs, int n)
    {
        return NULL;
    }
private:
};

GPPiecesFunction* GPBasicPiecesFunctionCreator::vCreateFromFuncTree(const GPFunctionTree* tree, const IParallelMachine* machine) const
{
    GPASSERT(NULL!=tree);
    GPASSERT(NULL!=machine);
    GPASSERT(tree->root()->type() == GPFunctionTreePoint::PARALLEL);
    return NULL;
}
GPBasicPiecesFunctionCreator::GPBasicPiecesFunctionCreator()
{
    
}
GPBasicPiecesFunctionCreator::~GPBasicPiecesFunctionCreator()
{
    
}
