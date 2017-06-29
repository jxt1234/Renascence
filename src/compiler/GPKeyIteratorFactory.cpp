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
#include "compiler/GPKeyIteratorFactory.h"
#include "GPBasicKeyIterator.h"
#include "GPOptimizedKeyIterator.h"
#include "utils/GPDebug.h"
#include "math/GPSingleTree.h"
#include <sstream>
#include <string>
#include "math/GPSingleTree.h"
GPKeyIteratorFactory::GPKeyIteratorFactory(const GPParallelType* type)
{
    GPASSERT(NULL!=type);
    mCanOptimize = false;
    mOutputKeys = type->mOutputKey;
    mConditionFunction = NULL;
    mCondition = type->sConditionInfo.sConditionFormula;
    
    static const char* optimzeFunction[] = {
        "==",
        "&&"
    };
    int size = sizeof(optimzeFunction)/sizeof(const char*);
    std::set<int> optimizeFunctionInt;
    for (int i=0; i<size; ++i)
    {
        optimizeFunctionInt.insert(GPSingleTreeFunction::func(optimzeFunction[i]));
    }
    
    if (type->sConditionInfo.sConditionFormula.size() > 0)
    {
        /*Translate variable info to keys*/
        std::string v;
        std::istringstream inputStream(type->sVariableInfo);
        std::map<int, std::pair<unsigned int, unsigned int>> keyMaps;
        int pos = 0;
        while (inputStream >> v)
        {
            unsigned int first = v[0] - 'a';
            unsigned int second = 0;
            for (int i=1; i<v.size(); ++i)
            {
                second = second*10 + (v[i]-'0');
            }
            keyMaps.insert(std::make_pair(pos++, (std::make_pair(first, second))));
        }
        
        /*Check the function tree*/
        GPPtr<GPSingleTree> tree = GPSingleTree::createFromFormula(type->sConditionInfo.sConditionFormula, type->sVariableInfo);
        auto allFunctions = tree->getAllFunction();
        auto allVariable = tree->getAllInput();
        for (auto vi : allVariable)
        {
            mInputsInConditionKeys.push_back(keyMaps.find(vi)->second);
        }
        mCanOptimize = true;
        for (auto f : allFunctions)
        {
            if (optimizeFunctionInt.find(f) == optimizeFunctionInt.end())
            {
                mCanOptimize = false;
                break;
            }
        }
        mConditionFunction = GPSingleTree::createFromFormula(type->sConditionInfo.sConditionFormula, type->sVariableInfo);
    }
}
GPKeyIteratorFactory::~GPKeyIteratorFactory()
{
    if (NULL != mConditionFunction)
    {
        mConditionFunction->decRef();
    }
}

IGPKeyIterator* GPKeyIteratorFactory::create(GPPieces** inputs, unsigned int nInput, GPPieces* output) const
{
    if (mCanOptimize)
    {
        return new GPOptimizedKeyIterator(inputs, nInput, mOutputKeys, mInputsInConditionKeys);
    }
    return new GPBasicKeyIterator(inputs, nInput, mOutputKeys, mConditionFunction);
}
