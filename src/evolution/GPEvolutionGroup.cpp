/******************************************************************
   Copyright 2014, Jiang Xiao-tang

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
#include <fstream>
#include <sstream>
#include <iostream>
#include "utils/GPDebug.h"
#include "utils/GPRandom.h"
#include "evolution/GPEvolutionGroup.h"
#include "optimizor/GPOptimizorFactory.h"
#include "frontend/GPFrontEndProducer.h"
#include "backend/GPBackEndProducer.h"
using namespace std;

void GPEvolutionGroup::func_para::init(IGPAutoDefFunction* function, GPFunctionTree* basic, const GPProducer* sys)
{
    GPASSERT(NULL!=basic);
    bool changed;//Not used, just for api
    auto sn = sys->getFront()->vMapStructure(basic, NULL, changed);
    pStructure = new GPParameter(sn);
    pStructure->clear(0.0f);
    auto pn = function->vMap(NULL);
    pParamter = new GPParameter(pn);
    for (int i=0; i<pParamter->size(); ++i)
    {
        pParamter->attach()[i] = GPRandom::rate();
    }
}
void GPEvolutionGroup::func_para::mutate()
{
    const double varyRate = 0.1f;
    auto n = pStructure->size();
    auto f = pStructure->attach();
    for (int i=0; i<n; ++i)
    {
        f[i] += varyRate * (GPRandom::rate()-0.5);
    }
    n = pParamter->size();
    f = pParamter->attach();
    for (int i=0; i<n; ++i)
    {
        f[i] += varyRate * (GPRandom::rate()-0.5);
    }
    pStructure->makeValid();
    pParamter->makeValid();
}
GPPtr<IGPAutoDefFunction> GPEvolutionGroup::func_para::invalidate(GPPtr<IGPAutoDefFunction> function, GPFunctionTree* basic, const GPProducer* sys)
{
    GPASSERT(NULL!=basic);
    bool changed;
    sys->getFront()->vMapStructure(basic, pStructure.get(), changed);
    if (changed)
    {
        /*Randomly initialize*/
        function = sys->getBack()->vCreateFromFuncTree(basic);
        auto pn = function->vMap(NULL);
        GPPtr<GPParameter> new_p = new GPParameter(pn);
        for (int i=0; i<new_p->size(); ++i)
        {
            new_p->attach()[i] = GPRandom::rate();
        }
        function->vMap(new_p.get());
    }
    else
    {
        function->vMap(pParamter.get());
    }
    return function;
}


GPEvolutionGroup::GPEvolutionGroup(GPProducer* sys, int time, int size):mSys(sys), mBestWrap(NULL,NULL)
{
    GPASSERT(NULL!=sys);
    mTime = time;
    mSize = size;
    GPRandom::init();
}

GPEvolutionGroup::~GPEvolutionGroup()
{
}

void GPEvolutionGroup::_best(std::function<double(IGPAutoDefFunction*)>& f)
{
    GPASSERT(mGroup.size()>1);
    double _max = f((*mGroup.begin())->mFunction.get());
    auto iter = mGroup.begin();
    auto best = mGroup[0];
    for (iter++; iter!=mGroup.end(); iter++)
    {
        double _f = f((*iter)->mFunction.get());
        if (_f > _max)
        {
            best = *iter;
            _max = _f;
        }
    }
    bool changed = true;
    if (mBestWrap.mFunction.get()!=NULL)
    {
        if (mBestFit <= _max)
        {
            mBestWrap.mFunction = best->mFunction->vCopy();
            mBestWrap.mTree = GPFunctionTree::copy(best->mTree.get());
            mBestFit = _max;
        }
        else
        {
            changed = false;
        }
    }
    else
    {
        mBestWrap.mFunction = best->mFunction->vCopy();
        mBestWrap.mTree = GPFunctionTree::copy(best->mTree.get());
        mBestFit = _max;
    }
    if (changed)
    {
        mPara = new func_para;
        mPara->init(mBestWrap.mFunction.get(), mBestWrap.mTree.get(), mSys);
    }
}

void GPEvolutionGroup::_expand()
{
    mGroup.clear();
    for (int i=0; i<mSize; ++i)
    {
        mGroup.push_back(new GPProducer::FunctionWrap(mBestWrap.mFunction->vCopy(), GPFunctionTree::copy(mBestWrap.mTree.get())));
    }
}

void GPEvolutionGroup::_mutate()
{
    GPASSERT(NULL!=mPara.get());
    for (auto f:mGroup)
    {
        mPara->mutate();
        f->mFunction = mPara->invalidate(f->mFunction, f->mTree.get(), mSys);
    }
}

void GPEvolutionGroup::loadBest(const GPTreeNode* node)
{
    GPASSERT(NULL!=mSys);
    mBestWrap.mFunction = mSys->createFunction(node);
}

void GPEvolutionGroup::vEvolutionFunc(std::function<double(IGPAutoDefFunction*)> fit_func)
{
    if (NULL!=mBestWrap.mFunction.get())
    {
        mBestFit = fit_func(mBestWrap.mFunction.get());
    }
    /*Create the initial group*/
    mGroup.clear();
    auto group = mSys->listAllFunctionWithBackUp(mOutput, mInput);
    GPASSERT(!group.empty());
    for (int i=0; i<group.size() && i<mSize; ++i)
    {
        mGroup.push_back(group[i]);
    }
    group.clear();
    _best(fit_func);
    _expand();
    /*Evolution*/
    for (int i=0; i<mTime; ++i)
    {
        _mutate();
        _best(fit_func);
        _expand();
    }
}
