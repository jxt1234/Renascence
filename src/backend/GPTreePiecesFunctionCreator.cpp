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
#include <sstream>
#include "backend/GPTreePiecesFunctionCreator.h"
#include "xml/xmlReader.h"

GPTreePiecesFunctionCreator::GPTreePiecesFunctionCreator(const GPFunctionDataBase* base, const IGPFunctionContext* context, const GPFrontEndProducer* front, GPStream* meta)
{
    GPASSERT(NULL!=base);
    GPASSERT(NULL!=meta);
    GPASSERT(NULL!=front);
    xmlReader reader;
    const GPTreeNode* root = reader.loadStream(meta);
    GPASSERT(NULL!=root);
    for (auto p : root->getChildren())
    {
        GPASSERT(p->getChildren().size() == 0);
        auto funcName = p->name();
        auto formula = p->attr();
        auto func = base->vQueryFunction(funcName);
        GPASSERT(NULL!=func);
        GPPtr<GPFunctionTree> tree = front->vCreateFromFormula(formula, std::vector<const IStatusType*>());
        mPieces.insert(std::make_pair(func, tree));
    }
    mBasic = new GPBasicPiecesFunctionCreator(context);
}
GPTreePiecesFunctionCreator::~GPTreePiecesFunctionCreator()
{
}


bool GPTreePiecesFunctionCreator::_checkValidTree(const GPFunctionTree* tree) const
{
    auto allPoints = tree->root()->display();
    for (auto p : allPoints)
    {
        const GPFunctionTreePoint* _p = GPCONVERT(const GPFunctionTreePoint, p);
        if (_p->type() == GPFunctionTreePoint::PARALLEL)
        {
            return false;
        }
        if (_p->type() == GPFunctionTreePoint::FUNCTION)
        {
            if (mPieces.find(_p->data().pFunc) == mPieces.end())
            {
                return false;
            }
        }
    }
    return true;
}

static std::string _getFunctionExpr(const GPFunction* function)
{
    std::ostringstream output;
    output << function->shortname <<"(";
    for (int i=0; i<function->inputType.size(); ++i)
    {
        output << "x" << i;
        if (i!=function->inputType.size()-1)
        {
            output << ",";
        }
    }
    output << ")";
    return output.str();
}
GPFunctionTreePoint* GPTreePiecesFunctionCreator::_transform(const GPFunctionTreePoint* p) const
{
    GPASSERT(NULL!=p);
    GPASSERT(GPFunctionTreePoint::FUNCTION == p->type() || p->getChildrenNumber() <= 0);
    if (GPFunctionTreePoint::FUNCTION != p->type())
    {
        return new GPFunctionTreePoint(*p);
    }
    GPASSERT(mPieces.find(p->data().pFunc)!=mPieces.end());
    GPFunctionTreePoint* copyMap = GPFunctionTree::copy(mPieces.find(p->data().pFunc)->second->root());
    auto inputs = GPFORCECONVERT(GPFunctionTreePoint, copyMap->getChild(0));
    GPASSERT(inputs->getChildrenNumber() == p->getChildrenNumber());
    for (int i=0; i<inputs->getChildrenNumber(); ++i)
    {
        auto ip = GPFORCECONVERT(GPFunctionTreePoint, inputs->getChild(i));
        auto newP = _transform(GPCONVERT(const GPFunctionTreePoint, p->getChild(i)));
        inputs->replace(ip, newP);
        newP->decRef();
    }
    return copyMap;
}



GPPiecesFunction* GPTreePiecesFunctionCreator::vCreateFromFuncTree(const GPFunctionTree* tree, const IParallelMachine* machine) const
{
    bool valid = _checkValidTree(tree);
    //TODO
    GPASSERT(valid);
    GPPtr<GPFunctionTree> treatedTree = new GPFunctionTree(_transform(tree->root()));
    return mBasic->vCreateFromFuncTree(treatedTree.get(), machine);
}
