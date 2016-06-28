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
#ifndef BACKEND_GPDAGPIECESFUNCTIONCREATOR_H
#define BACKEND_GPDAGPIECESFUNCTIONCREATOR_H
#include <map>
#include "backend/GPPiecesFunctionCreator.h"
#include "backend/GPTreePiecesFunctionCreator.h"
#include "frontend/GPFrontEndProducer.h"
class GPDAGPiecesFunctionCreator:public GPTreePiecesFunctionCreator
{
public:
    GPDAGPiecesFunctionCreator(const GPFunctionDataBase* base, const IGPFunctionContext* context, const GPFrontEndProducer* front, const std::map<std::string, std::string>& map_reduce_formula);
    virtual ~GPDAGPiecesFunctionCreator();
    virtual GPPiecesFunction* vCreateFromFuncTree(const GPFunctionTree* tree, const IParallelMachine* machine) const;
};

#endif