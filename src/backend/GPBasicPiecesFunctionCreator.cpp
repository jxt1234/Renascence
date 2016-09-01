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
#include "frontend/GPFrontEndProducer.h"
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


/*For cache*/
class PieceCreator:public GPRefCount
{
public:
    PieceCreator(const std::vector<std::pair<unsigned int, unsigned int>>& outputKeys, const std::vector<const IStatusType*>& types, const IParallelMachine* machine, IParallelMachine::USAGE usage, const char* path)
    {
        if (NULL == path)
        {
            mPath = "";
        }
        else
        {
            mPath = path;
        }
        mTypes = types;
        mMachine = machine;
        mOutputKey = outputKeys;
        mUsage = usage;
    }
    ~ PieceCreator(){}
    
    virtual GPPieces* vPrepare(GPPieces** inputs, int inputNumber) const
    {
        //GPCLOCK;
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
        if (keyDms.empty())
        {
            keyDms.push_back(1);
        }
        AUTOSTORAGE(keyDims, unsigned int, (int)keyDms.size());
        for (int i=0; i<keyDms.size(); ++i)
        {
            keyDims[i] = keyDms[i];
        }
//        for (auto t : mTypes)
//        {
//            FUNC_PRINT_ALL(t->name().c_str(), s);
//        }
//        FUNC_PRINT(mTypes.size());
        return mMachine->vCreatePieces(mPath.c_str(), mTypes, keyDims, (int)keyDms.size(), mUsage);
    }
private:
    std::vector<std::pair<unsigned int, unsigned int>> mOutputKey;
    std::vector<const IStatusType*> mTypes;
    const IParallelMachine* mMachine;
    IParallelMachine::USAGE mUsage;
    std::string mPath;
};

class GPPiecesFunctionNode : public GPAbstractPoint
{
public:
    GPPiecesFunctionNode(GPPtr<PieceCreator> creator, GPPtr<IParallelMachine::Executor> executor)
    {
        GPASSERT(NULL!=creator.get() && NULL!=executor.get());
        mExecutor = executor;
        mInputPos = -1;
        mCreator = creator;
    }
    
    GPPiecesFunctionNode(int inputPos)
    {
        GPASSERT(inputPos>=0);
        mInputPos = inputPos;
    }
    
    void setDescription(const std::string& description)
    {
        mDescription = description;
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
        //FUNC_PRINT_ALL(mDescription.c_str(), s);
        int subNum = (int)mChildren.size();
        AUTOSTORAGE(subinputs, GPPieces*, subNum);
        AUTOSTORAGE(needClean, bool, subNum);
        for (int i=0; i<subNum; ++i)
        {
            subinputs[i] = NULL;
            GPPiecesFunctionNode* node = GPCONVERT(GPPiecesFunctionNode, mChildren[i]);
            subinputs[i] = node->compute(inputs, n);
            needClean[i] = false;
            if (node->mInputPos < 0)
            {
                /*The Content is from compute, must clean*/
                needClean[i] = true;
            }
        }
        auto output = mCreator->vPrepare(subinputs, subNum);
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
    
    static std::vector<std::pair<unsigned int, unsigned int>> translate(const std::string& variables)
    {
        std::vector<std::pair<unsigned int, unsigned int>> result;
        std::string s;
        std::istringstream input(variables);
        while (input >> s)
        {
            GPASSERT(s.size()>=2);
            int first = s[0] - 'x';
            GPASSERT(first>=0);
            int second = 0;
            for (int i=1; i<s.size(); ++i)
            {
                second = second*10 + (s[i]-'0');
            }
            result.push_back(std::make_pair(first, second));
        }
        return result;
    }
    
    
    static GPPiecesFunctionNode* createFromFuncPoint(const GPFunctionTreePoint* root, const IParallelMachine* machine, const GPProducer* context)
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
        GPASSERT(func->getChildrenNumber() == 2);
        data.sFuncInfo.variableKey = translate(GPCONVERT(const GPFunctionTreePoint, func->getChild(0))->extra());
        data.sFuncInfo.formula = GPCONVERT(const GPFunctionTreePoint, func->getChild(1))->extra();
        //TODO
        data.sFuncInfo.parameter = "";
        data.sFuncInfo.inputs.clear();
        
        GPPtr<GPFunctionTree> tree = context->getFront()->vCreateFromFormula(data.sFuncInfo.formula, data.sFuncInfo.inputs);
        GPASSERT(tree->root()->type() == GPFunctionTreePoint::FUNCTION);//TODO, currently don't support multi output
        std::vector<const IStatusType*> outputTypes = tree->root()->data().pFunc->outputType;
        
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
        GPPtr<PieceCreator> pieceCreator = new PieceCreator(data.mOutputKey, outputTypes, machine, IParallelMachine::CACHE, NULL);
        GPPtr<IParallelMachine::Executor> executor = machine->vPrepare(&data, root->data().iParallelType);
        GPPiecesFunctionNode* result = new GPPiecesFunctionNode(pieceCreator, executor);
        GPASSERT(root->getChildrenNumber() >= 2);
        {
            auto inputs = root->getChild(0);
            for (int i=0; i<inputs->getChildrenNumber(); ++i)
            {
                auto p = GPCONVERT(const GPFunctionTreePoint, inputs->getChild(i));
                result->addPoint(createFromFuncPoint(p, machine, context));
            }
        }
        //FUNC_PRINT_ALL(data.sFuncInfo.formula.c_str(), s);
        result->setDescription(data.sFuncInfo.formula);
        return result;
    }
    
private:
    GPPtr<PieceCreator> mCreator;
    GPPtr<IParallelMachine::Executor> mExecutor;
    int mInputPos;
    std::string mDescription;
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
    
    virtual GPPieces* vRun(GPPieces** inputs, int n) override
    {
        GPASSERT(NULL!=inputs);
        GPASSERT(n>=1);
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
    //FUNC_PRINT_ALL(tree->dump().c_str(), s);
    GPPiecesFunctionNode* node = GPPiecesFunctionNode::createFromFuncPoint(root, machine, mContext);
    return new GPBasicPiecesFunction(node);
}
GPBasicPiecesFunctionCreator::GPBasicPiecesFunctionCreator(const GPProducer* context)
{
    mContext = context;
}
GPBasicPiecesFunctionCreator::~GPBasicPiecesFunctionCreator()
{
    
}
