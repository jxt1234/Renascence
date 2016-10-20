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
#ifndef BACKEND_GPKEYITERATORFACTORY_H
#define BACKEND_GPKEYITERATORFACTORY_H
#include "IGPKeyIterator.h"
#include "lowlevelAPI/IParallelMachine.h"
class GPKeyIteratorFactory:public GPRefCount
{
public:
    GPKeyIteratorFactory(const GPParallelType* type);
    virtual ~ GPKeyIteratorFactory();
    
    IGPKeyIterator* create(GPPieces** inputs, unsigned int nInput, GPPieces* output) const;
private:
    std::string mCondition;
    GPParallelType::KEYS mInputsInConditionKeys;
    IKeyFunction* mConditionFunction;
    GPParallelType::KEYS mOutputKeys;
    bool mCanOptimize;
};
#endif
