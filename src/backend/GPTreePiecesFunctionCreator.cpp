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

GPTreePiecesFunctionCreator::GPTreePiecesFunctionCreator(const GPFunctionDataBase* base, const IGPFunctionContext* context, const GPFrontEndProducer* front, const std::map<std::string, std::string>& map_reduce_formula)
{
    GPASSERT(NULL!=base);
    GPASSERT(NULL!=front);
    for (auto iter : map_reduce_formula)
    {
        auto funcName = iter.first;
        auto formula = iter.second;
        auto func = base->vQueryFunction(funcName);
        GPASSERT(NULL!=func);
        GPPtr<GPFunctionTree> tree = front->vCreateFromFormula(formula, std::vector<const IStatusType*>());
        //FUNC_PRINT_ALL(tree->dump().c_str(), s);
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
        if (GPFunctionTreePoint::PARALLEL == _p->type())
        {
            return false;
        }
        if (GPFunctionTreePoint::FUNCTION == _p->type())
        {
            if (mPieces.find(_p->data().pFunc) == mPieces.end())
            {
                return false;
            }
        }
    }
    return true;
}


static void _replaceLeafInputs(GPFunctionTreePoint* p, const std::map<int, GPFunctionTreePoint*>& replaceMap)
{
    GPASSERT(NULL!=p);
    GPASSERT(GPFunctionTreePoint::STRING == p->type() && p->extra() == "INPUT");
    for (int i=0; i<p->getChildrenNumber(); ++i)
    {
        auto pc = GPFORCECONVERT(GPFunctionTreePoint, p->getChild(i));
        if (GPFunctionTreePoint::PARALLEL == pc->type())
        {
            _replaceLeafInputs(GPFORCECONVERT(GPFunctionTreePoint, pc->getChild(0)), replaceMap);
            continue;
        }
        GPASSERT(GPFunctionTreePoint::INPUT == pc->type());
        auto copy = GPFunctionTree::copy(replaceMap.find(pc->data().iInput)->second);
        p->replace(pc, copy);
        copy->decRef();
    }
}

GPFunctionTreePoint* GPTreePiecesFunctionCreator::_transform(const GPFunctionTreePoint* p) const
{
    GPASSERT(NULL!=p);
    GPASSERT(GPFunctionTreePoint::FUNCTION == p->type() || p->getChildrenNumber() <= 0);
    if (GPFunctionTreePoint::FUNCTION != p->type())
    {
        return new GPFunctionTreePoint(*p);
    }
    //FUNC_PRINT_ALL(p->data().pFunc->name.c_str(), s);
    GPASSERT(mPieces.find(p->data().pFunc)!=mPieces.end());
    //FUNC_PRINT_ALL(mPieces.find(p->data().pFunc)->second->dump().c_str(), s);
    GPFunctionTreePoint* copyMap = GPFunctionTree::copy(mPieces.find(p->data().pFunc)->second->root());
    auto inputs = GPFORCECONVERT(GPFunctionTreePoint, copyMap->getChild(0));
    GPASSERT(inputs->getChildrenNumber() == p->getChildrenNumber());
    std::map<int, GPFunctionTreePoint*> replaceMap;
    for (int i=0; i<inputs->getChildrenNumber(); ++i)
    {
        auto newP = _transform(GPCONVERT(const GPFunctionTreePoint, p->getChild(i)));
        replaceMap.insert(std::make_pair(i, newP));
    }
    _replaceLeafInputs(inputs, replaceMap);
    for (auto k : replaceMap)
    {
        k.second->decRef();
    }
    return copyMap;
}



GPPiecesFunction* GPTreePiecesFunctionCreator::vCreateFromFuncTree(const GPFunctionTree* tree, const IParallelMachine* machine) const
{
    bool valid = _checkValidTree(tree);
    //TODO
    GPASSERT(valid);
    GPPtr<GPFunctionTree> treatedTree = new GPFunctionTree(_transform(tree->root()));
    //FUNC_PRINT_ALL(treatedTree->dump().c_str(), s);
    return mBasic->vCreateFromFuncTree(treatedTree.get(), machine);
}
