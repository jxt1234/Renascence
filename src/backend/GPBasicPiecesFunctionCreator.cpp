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
#include "backend/GPBasicPiecesFunctionCreator.h"
#include <sstream>

static std::pair<int, int> _translate(const std::string& name)
{
    GPASSERT(name.size()>=2);
    int a = name[0] - 'a';
    int b = 0;
    for (int i=1; i<name.size(); ++i)
    {
        b*=10;
        b+=name[i]-'0';
    }
    return std::make_pair(a, b);
}

static bool _isValid(const std::string& name)
{
    if (name.size()<2)
    {
        return false;
    }
    if (name[0]<'a' || name[0]>'z')
    {
        return false;
    }
    return true;
}

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
    
    static GPPiecesFunctionNode* createFromFuncPoint(const GPFunctionTreePoint* root, const IParallelMachine* machine, const IGPFunctionContext* context)
    {
        GPASSERT(NULL!=context);
        GPASSERT(GPFunctionTreePoint::PARALLEL == root->type() || GPFunctionTreePoint::INPUT == root->type());
        if (GPFunctionTreePoint::INPUT == root->type())
        {
            return new GPPiecesFunctionNode(root->data().iInput);
        }
        GPParallelType data;
        data.pContext = context;
        auto func = GPCONVERT(const GPFunctionTreePoint, root->getChild(1));
        GPASSERT(func->type() == GPFunctionTreePoint::STRING);
        data.sFuncInfo.formula = func->extra();
        //TODO
        data.sFuncInfo.parameter = "";
        data.sFuncInfo.inputs.clear();
        
        std::ostringstream inputVariables;
        if (root->getChildrenNumber()>2)
        {
            auto keymap_point = root->getChild(2);
            GPASSERT(keymap_point->getChildrenNumber() == 2);
            {
                //Output
                auto output = keymap_point->getChild(1);
                for (int i=0; i<output->getChildrenNumber(); ++i)
                {
                    auto p = GPCONVERT(const GPFunctionTreePoint, output->getChild(i));
                    GPASSERT(p->type() == GPFunctionTreePoint::STRING);
                    //TODO
                    if (_isValid(p->extra()))
                    {
                        data.mOutputKey.push_back(_translate(p->extra()));
                    }
                }
            }
            {
                //Input
                auto input = keymap_point->getChild(0);
                for (int i=0; i<input->getChildrenNumber(); ++i)
                {
                    auto p = GPCONVERT(const GPFunctionTreePoint, input->getChild(i));
                    GPASSERT(p->type() == GPFunctionTreePoint::STRING);
                    inputVariables << p->extra() << " ";
                }
                data.sVariableInfo = inputVariables.str();
            }
        }
        if (root->getChildrenNumber()>3)
        {
            auto condition = GPCONVERT(const GPFunctionTreePoint, root->getChild(3));
            data.sConditionInfo.sConditionFormula = condition->extra();
        }
        
        auto creator_executor = machine->vGenerate(&data, root->data().iParallelType);
        GPPiecesFunctionNode* result = new GPPiecesFunctionNode(creator_executor.first, creator_executor.second);
        GPASSERT(root->getChildrenNumber() >= 2);
        {
            auto inputs = root->getChild(0);
            for (int i=0; i<inputs->getChildrenNumber(); ++i)
            {
                auto p = GPCONVERT(const GPFunctionTreePoint, inputs->getChild(i));
                result->addPoint(createFromFuncPoint(p, machine, context));
            }
        }
        return result;
        
    }
    
private:
    GPPtr<IParallelMachine::Creator> mCreator;
    GPPtr<IParallelMachine::Executor> mExecutor;
    int mInputPos;
};


class GPBasicPiecesFunction : public GPPiecesFunction
{
public:
    GPBasicPiecesFunction(GPPtr<GPPiecesFunctionNode> root)
    {
        mRoot = root;
    }
    virtual ~GPBasicPiecesFunction()
    {
    }
    
    virtual GPPieces* vRun(GPPieces** inputs, int n)
    {
        return mRoot->compute(inputs, n);
    }
private:
    GPPtr<GPPiecesFunctionNode> mRoot;
};

GPPiecesFunction* GPBasicPiecesFunctionCreator::vCreateFromFuncTree(const GPFunctionTree* tree, const IParallelMachine* machine) const
{
    GPASSERT(NULL!=tree);
    GPASSERT(NULL!=machine);
    GPASSERT(tree->root()->type() == GPFunctionTreePoint::PARALLEL);
    auto root = tree->root();
    GPPiecesFunctionNode* node = GPPiecesFunctionNode::createFromFuncPoint(root, machine, mContext);
    return new GPBasicPiecesFunction(node);
}
GPBasicPiecesFunctionCreator::GPBasicPiecesFunctionCreator(const IGPFunctionContext* context)
{
    mContext = context;
}
GPBasicPiecesFunctionCreator::~GPBasicPiecesFunctionCreator()
{
    
}
