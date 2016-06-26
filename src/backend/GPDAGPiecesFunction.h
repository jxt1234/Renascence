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
#ifndef BACKEND_GPDAGPIECESFUNCTION_H
#define BACKEND_GPDAGPIECESFUNCTION_H
#include <map>
#include "lowlevelAPI/GPPiecesFunction.h"
#include "core/GPFunctionTree.h"
#include "backend/GPPiecesFunctionCreator.h"
class GPDAGPiecesFunction:public GPPiecesFunction
{
public:
    typedef std::map<int, GPPtr<GPFunctionTree>> TREES;
    GPDAGPiecesFunction(const TREES& p, const GPPiecesFunctionCreator* creator, const IParallelMachine* machine);
    virtual ~GPDAGPiecesFunction();
    virtual GPPieces* vRun(GPPieces** inputs, int n);
private:
    std::vector<GPPiecesFunction*> mFunctions;//Ordered
    std::map<GPPiecesFunction*, int> mFunctionComputeOutputMap;
    int mMaxInputPos;
    
    std::vector<GPPtr<GPPiecesFunction>> mHoldedFunction;//Use to storage and release functions
};
#endif
