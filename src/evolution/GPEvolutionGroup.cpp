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

void GPEvolutionGroup::func_para::init(IGPAutoDefFunction* function, const GPProducer* sys)
{
    GPASSERT(NULL!=function);
    GPFunctionTree* basic = function->getBasicTree().get();
    bool changed;//Not used, just for api
    auto sn = sys->getFront()->vMapStructure(basic, NULL, 0, changed);
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
GPPtr<IGPAutoDefFunction> GPEvolutionGroup::func_para::invalidate(GPPtr<IGPAutoDefFunction> function, const GPProducer* sys, int depth)
{
    bool changed;
    auto basic = function->getBasicTree();
    sys->getFront()->vMapStructure(basic.get(), pStructure.get(), depth, changed);
    if (changed)
    {
        /*Randomly initialize*/
        function = sys->getBack()->vCreateFromFuncTree(basic.get());
        function->setBasicTree(basic);
        auto pn = function->vMap(NULL);
        GPPtr<GPParameter> new_p = new GPParameter(pn);
        for (int i=0; i<new_p->size(); ++i)
        {
            new_p->attach()[i] = GPRandom::rate();
        }
        function->vMap(new_p.get());
        function->setParameters(new_p);
    }
    else
    {
        function->vMap(pParamter.get());
        function->setParameters(pParamter->copy());
    }
    return function;
}


GPEvolutionGroup::GPEvolutionGroup(GPProducer* sys, int time, int size, int depth):mSys(sys)
{
    GPASSERT(NULL!=sys);
    GPASSERT(time > 0);
    GPASSERT(size > 0);
    mTime = time;
    mSize = size;
    mDepth = depth;
    GPRandom::init();
}

GPEvolutionGroup::~GPEvolutionGroup()
{
}

void GPEvolutionGroup::_best(std::function<double(IGPAutoDefFunction*)>& f)
{
    GPASSERT(mGroup.size()>=1);
    double _max = f((mGroup.begin()->get()));
    auto iter = mGroup.begin();
    auto best = mGroup[0];
    for (iter++; iter!=mGroup.end(); iter++)
    {
        if (NULL!=iter->get()->getParameters().get())
        {
            GPASSERT(iter->get()->getParameters()->size() == iter->get()->vMap(NULL));
        }
        double _f = f(iter->get());
        if (_f > _max)
        {
            best = *iter;
            _max = _f;
        }
    }
    bool changed = true;
    if (mBest.get()!=NULL)
    {
        if (mBestFit <= _max)
        {
            mBest = mSys->getBack()->vCreateFromFuncTree(best->getBasicTree().get());
            mBest->setBasicTree(GPFunctionTree::copy(best->getBasicTree().get()));
            if (NULL!=best->getParameters().get())
            {
                mBest->setParameters(best->getParameters()->copy());
                mBest->vMap(best->getParameters().get());
            }
            mBestFit = _max;
        }
        else
        {
            changed = false;
        }
    }
    else
    {
        mBest = mSys->getBack()->vCreateFromFuncTree(best->getBasicTree().get());
        mBest->setBasicTree(GPFunctionTree::copy(best->getBasicTree().get()));
        if (NULL!=best->getParameters().get())
        {
            mBest->setParameters(best->getParameters()->copy());
            mBest->vMap(best->getParameters().get());
        }
        mBestFit = _max;
    }
    if (changed)
    {
        mPara = new func_para;
        mPara->init(mBest.get(), mSys);
    }
}

void GPEvolutionGroup::_expand()
{
    mGroup.clear();
    for (int i=0; i<mSize; ++i)
    {
        GPPtr<IGPAutoDefFunction> adf = mSys->getBack()->vCreateFromFuncTree(mBest->getBasicTree().get());
        adf->setBasicTree(GPFunctionTree::copy(mBest->getBasicTree().get()));
        if (NULL!=mBest->getParameters().get())
        {
            adf->setParameters(mBest->getParameters()->copy());
            adf->vMap(adf->getParameters().get());
        }
        mGroup.push_back(adf);
    }
}

void GPEvolutionGroup::_mutate()
{
    GPASSERT(NULL!=mPara.get());
    for (auto& f:mGroup)
    {
        mPara->mutate();
        f = mPara->invalidate(f, mSys, mDepth);
    }
}

/*TODO*/
void GPEvolutionGroup::loadBest(const GPTreeNode* node)
{
    GPASSERT(NULL!=mSys);
    mBest = mSys->createFunction(node);
}

void GPEvolutionGroup::vEvolutionFunc(std::function<double(IGPAutoDefFunction*)> fit_func, GPWStream* cache)
{
    if (NULL!=mBest.get())
    {
        if (NULL == mPara.get())
        {
            mPara = new func_para;
            mPara->init(mBest.get(), mSys);
        }
        mBestFit = fit_func(mBest.get());
    }
    else
    {
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
    }
    _expand();
    /*Evolution*/
    for (int i=0; i<mTime; ++i)
    {
        if (NULL!=cache)
        {
            std::ostringstream total;
            total << mBestFit <<",";
            auto param = mBest->getParameters();
            if (NULL!=param.get())
            {
                auto n = param->size();
                for (int i=0; i<n; ++i)
                {
                    total << param->get(i) << " ";
                }
            }
            auto formula = mBest->getBasicTree()->dump();
            total << ", "<<formula<<"\n";
            cache->vWrite(total.str().c_str(), total.str().size());
        }
        _mutate();
        _best(fit_func);
        _expand();
    }
}
void GPEvolutionGroup::setBestTree(GPPtr<GPFunctionTree> tree)
{
    GPASSERT(NULL!=tree.get());
    mBest = mSys->getBack()->vCreateFromFuncTree(tree.get());
    mBest->setBasicTree(tree);
    int n = mBest->vMap(NULL);
    if (n>0)
    {
        GPPtr<GPParameter> par = new GPParameter(n);
        mBest->setParameters(par);
        mBest->vMap(par.get());
    }
}

