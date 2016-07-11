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
#include "GPDAGPiecesFunction.h"
#include <string.h>
#include <algorithm>
#include <list>
#include <set>

GPDAGPiecesFunction::GPDAGPiecesFunction(const TREES& p, const GPPiecesFunctionCreator* creator, const IParallelMachine* machine)
{
    typedef std::pair<std::vector<int>, GPPiecesFunction*> FUNCINDEX;
    std::map<int, GPPiecesFunction*> functionInputObtainMap;
    std::list<FUNCINDEX> orderedFunctionBefore;
    mMaxInputPos = -1;
    for (auto kv : p)
    {
        int maxInputPos = kv.second->root()->maxInputPos();
        if (mMaxInputPos < maxInputPos)
        {
            mMaxInputPos = maxInputPos;
        }
        auto mapInputs = kv.second->root()->getInputPos();
        std::vector<int> allInputs;
        for (auto iter : mapInputs)
        {
            allInputs.push_back(iter);
        }
        GPPtr<GPPiecesFunction> function = creator->vCreateFromFuncTree(kv.second.get(), machine);
        mHoldedFunction.push_back(function);
        functionInputObtainMap.insert(std::make_pair(kv.first, function.get()));
        orderedFunctionBefore.push_back(std::make_pair(allInputs, function.get()));
        mFunctionComputeOutputMap.insert(std::make_pair(function.get(), kv.first));
    }
    while (!orderedFunctionBefore.empty())
    {
        for (auto iter = orderedFunctionBefore.begin(); iter != orderedFunctionBefore.end(); iter++)
        {
            bool find = false;
            for (int i=0; i<iter->first.size(); ++i)
            {
                auto obtain = functionInputObtainMap.find(iter->first[i]);
                if (obtain != functionInputObtainMap.end())
                {
                    find = true;
                    break;
                }
            }
            if (!find)
            {
                mFunctions.push_back(iter->second);
                int outputPos = mFunctionComputeOutputMap.find(iter->second)->second;
                functionInputObtainMap.erase(outputPos);
                orderedFunctionBefore.erase(iter);
                break;
            }
        }
    }
    /*Turn to maxInputNumber*/
    mMaxInputPos += 1;
}

GPDAGPiecesFunction::~GPDAGPiecesFunction()
{
}

GPPieces* GPDAGPiecesFunction::vRun(GPPieces** inputs, int n)
{
    //GPCLOCK;
    GPASSERT(mMaxInputPos >= n);
    AUTOSTORAGE(inputsEnlarge, GPPieces*, mMaxInputPos);
    ::memset(inputsEnlarge, 0, mMaxInputPos*sizeof(GPPieces*));
    ::memcpy(inputsEnlarge, inputs, n*sizeof(GPPieces*));
    GPPieces* result = NULL;
    for (int i=0; i<mFunctions.size(); ++i)
    {
        GPPieces* output = mFunctions[i]->vRun(inputsEnlarge, mMaxInputPos);
        auto outputPos = mFunctionComputeOutputMap.find(mFunctions[i])->second;
        if (-1 == outputPos)
        {
            result = output;
            break;
        }
        GPASSERT(NULL == inputsEnlarge[outputPos]);
        inputsEnlarge[outputPos] = output;
    }
    GPASSERT(NULL!=result);
    /*Release temp result*/
    for (int i=n; i<mMaxInputPos; ++i)
    {
        if (NULL!=inputsEnlarge[i])
        {
            inputsEnlarge[i]->decRef();
            inputsEnlarge[i] = NULL;
        }
    }
    return result;
}
