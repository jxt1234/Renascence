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

#include "core/GPProducer.h"
#include "backend/GPBackEndProducer.h"
#include "frontend/GPFrontEndProducer.h"

GPProducer::GPProducer(GPFrontEndProducer* front, GPBackEndProducer* back, const GPFunctionDataBase* base)
{
    GPASSERT(NULL!=front);
    GPASSERT(NULL!=back);
    GPASSERT(NULL!=base);
    mFront = front;
    mBack = back;
    mFront->addRef();
    mBack->addRef();
    mBase = base;
}
GPProducer::~GPProducer()
{
    mFront->decRef();
    mBack->decRef();
}

IGPAutoDefFunction* GPProducer::createFunction(const std::vector<const IStatusType*>& outputs, const std::vector<const IStatusType*>& inputs)
{
    GPPtr<GPFunctionTree> ft = mFront->vCreateOneFunction(outputs,inputs);
    return mBack->vCreateFromFuncTree(ft.get());
}
std::vector<GPPtr<IGPAutoDefFunction>> GPProducer::listAllFunction(const std::vector<const IStatusType*>& outputs, const std::vector<const IStatusType*>& inputs, int depth)
{
    auto fts = mFront->vCreateAllFunction(outputs, inputs, depth);
    std::vector<GPPtr<IGPAutoDefFunction> > result;
    for (auto f : fts)
    {
        result.push_back(mBack->vCreateFromFuncTree(f));
        f->decRef();
    }
    return result;
}
IGPAutoDefFunction* GPProducer::createFunction(const std::string& formula, const std::vector<const IStatusType*>& inputs)
{
    GPPtr<GPFunctionTree> ft = mFront->vCreateFromFormula(formula, inputs);
    return mBack->vCreateFromFuncTree(ft.get());
}
IGPAutoDefFunction* GPProducer::createFunction(const GPTreeNode* node)
{
    return mBack->vCreateFromNode(node, mBase);
}

std::vector<GPPtr<IGPAutoDefFunction>> GPProducer::listAllFunctionWithBackUp(const std::vector<const IStatusType*>& outputs, const std::vector<const IStatusType*>& inputs, int depth)
{
    std::vector<GPPtr<IGPAutoDefFunction>> result;
    auto fts = mFront->vCreateAllFunction(outputs, inputs, depth);
    for (auto f : fts)
    {
        GPPtr<GPFunctionTree> precompiled = f;
        GPPtr<IGPAutoDefFunction> adf = mBack->vCreateFromFuncTree(f);
        adf->setBasicTree(precompiled);
        int n = adf->vMap(NULL);
        if (n > 0)
        {
            GPPtr<GPParameter> initPara = new GPParameter(n);
            adf->setParameters(initPara);
            adf->vMap(initPara.get());
        }
        result.push_back(adf);
    }
    return result;
}

