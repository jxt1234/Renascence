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
#include "backend/GPKeyIteratorFactory.h"
#include "GPBasicKeyIterator.h"
#include "utils/GPDebug.h"
GPKeyIteratorFactory::GPKeyIteratorFactory(const GPParallelType* type)
{
    GPASSERT(NULL!=type);
    mOutputKeys = type->mOutputKey;
    mConditionFunction = NULL;
    if (type->sConditionInfo.sConditionFormula.size() > 0)
    {
        mConditionFunction = type->pContext->vCreateFloatFunction(type->sConditionInfo.sConditionFormula, type->sVariableInfo);
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
    return new GPBasicKeyIterator(inputs, nInput, mOutputKeys, mConditionFunction);
}
