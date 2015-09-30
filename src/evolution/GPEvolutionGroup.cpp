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
#include "utils/debug.h"
#include "utils/GPRandom.h"
#include "evolution/GPEvolutionGroup.h"
#include "optimizor/GPOptimizorFactory.h"
using namespace std;

void GPEvolutionGroup::func_para::init(IGPAutoDefFunction* basic)
{
    GPASSERT(NULL!=basic);
    bool changed;
    auto sn = basic->vMapStructure(NULL, changed);
    pStructure = new GPParameter(sn);
    pStructure->clear(0.0f);
    auto pn = basic->vMap(NULL);
    pParamter = new GPParameter(pn);
    for (int i=0; i<pParamter->size(); ++i)
    {
        pParamter->attach()[i] = GPRandom::rate();
    }
    basic->vMap(pParamter.get());
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
void GPEvolutionGroup::func_para::invalidate(IGPAutoDefFunction* basic)
{
    GPASSERT(NULL!=basic);
    bool changed = false;
    basic->vMapStructure(pStructure.get(), changed);
    if (changed)
    {
        init(basic);
        return;
    }
    basic->vMap(pParamter.get());
}


GPEvolutionGroup::GPEvolutionGroup(GPProducer* sys, int time, int size):mSys(sys)
{
    GPASSERT(NULL!=sys);
    mBest = NULL;
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
    double _max = f((*(mGroup.begin())).get());
    auto iter = mGroup.begin();
    auto bestIter = iter;
    for (iter++; iter!=mGroup.end(); iter++)
    {
        double _f = f((*iter).get());
        if (_f > _max)
        {
            bestIter = iter;
            _max = _f;
        }
    }
    bool changed = true;
    if (mBest.get()!=NULL)
    {
        if (mBestFit <= _max)
        {
            mBest = (*bestIter)->vCopy();
            mBestFit = _max;
        }
        else
        {
            changed = false;
        }
    }
    else
    {
        mBest = (*bestIter)->vCopy();
        mBestFit = _max;
    }
    if (changed)
    {
        mPara = new func_para;
        mPara->init(mBest.get());
    }
}

void GPEvolutionGroup::_expand()
{
    mGroup.clear();
    for (int i=0; i<mSize; ++i)
    {
        mGroup.push_back(mBest->vCopy());
    }
}

void GPEvolutionGroup::_mutate()
{
    GPASSERT(NULL!=mPara.get());
    for (auto f:mGroup)
    {
        mPara->mutate();
        mPara->invalidate(f.get());
    }
}

void GPEvolutionGroup::loadBest(const GPTreeNode* node)
{
    GPASSERT(NULL!=mSys);
    mBest = mSys->vCreateFunctionFromNode(node);
}

void GPEvolutionGroup::vEvolutionFunc(std::function<double(IGPAutoDefFunction*)> fit_func)
{
    if (NULL!=mBest.get())
    {
        mBestFit = fit_func(mBest.get());
    }
    /*Create the initial group*/
    mGroup.clear();
    vector<IGPAutoDefFunction*> group = mSys->vCreateAllFunction(mOutput, mInput);
    GPASSERT(!group.empty());
    for (int i=0; i<group.size(); ++i)
    {
        mGroup.push_back(group[i]);
    }
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
