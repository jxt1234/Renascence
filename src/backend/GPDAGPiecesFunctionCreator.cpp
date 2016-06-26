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
#include "backend/GPDAGPiecesFunctionCreator.h"
#include "midend/GPMapReduceMerger.h"
#include "midend/GPMultiLayerTree.h"
#include "GPDAGPiecesFunction.h"


GPDAGPiecesFunctionCreator::GPDAGPiecesFunctionCreator(const GPFunctionDataBase* base, const IGPFunctionContext* context, const GPFrontEndProducer* front, const std::map<std::string, std::string>& map_reduce_formula):GPTreePiecesFunctionCreator(base, context, front, map_reduce_formula)
{
}

GPDAGPiecesFunctionCreator::~GPDAGPiecesFunctionCreator()
{
}

GPPiecesFunction* GPDAGPiecesFunctionCreator::vCreateFromFuncTree(const GPFunctionTree* tree, const IParallelMachine* machine) const
{
    bool valid = _checkValidTree(tree);
    //TODO
    GPASSERT(valid);
    GPMultiLayerTree _multi(tree);
    auto layers = _multi.layers();
    GPDAGPiecesFunction::TREES allTrees;
    for (auto p : layers)
    {
        GPPtr<GPFunctionTree> treatedTree = new GPFunctionTree(_transform(p.second.get()));
        treatedTree = GPMapReduceMerger::reduce(treatedTree.get());
        allTrees.insert(std::make_pair(p.first, treatedTree));
    }
    return new GPDAGPiecesFunction(allTrees, mBasic.get(), machine);
}
