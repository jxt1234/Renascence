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
    if (NULL!=mBest)
    {
        mBest->decRef();
    }
    _clearGroup();
    _clearBackup();
}

void GPEvolutionGroup::_clearBackup()
{
    vector<IGPAutoDefFunction*>::iterator iter = mBackup.begin();
    for (; iter!=mBackup.end(); iter++)
    {
        if (NULL != *iter)
        {
            (*iter)->decRef();
        }
    }
    mBackup.clear();
}

void GPEvolutionGroup::_restoreBackup()
{
    _clearBackup();
    list<IGPAutoDefFunction*>::iterator iter = mGroup.begin();
    for (; iter!=mGroup.end(); iter++)
    {
        mBackup.push_back((*iter)->copy());
    }
}

void GPEvolutionGroup::_clearGroup()
{
    list<IGPAutoDefFunction*>::iterator iter = mGroup.begin();
    for (; iter!=mGroup.end(); iter++)
    {
        if (NULL != *iter)
        {
            (*iter)->decRef();
        }
    }
    mGroup.clear();
}


void GPEvolutionGroup::_best(std::function<double(IGPAutoDefFunction*)> f)
{
    int bestId = 0;
    double _max = f(*(mGroup.begin()));
    list<IGPAutoDefFunction*>::iterator iter = mGroup.begin();
    list<IGPAutoDefFunction*>::iterator bestIter = mGroup.begin();
    for (iter++; iter!=mGroup.end(); iter++)
    {
        double _f = f(*iter);
        //static int count=0;
        //std::ostringstream os;
        //os << "output/"<<count++<<".txt";
        //std::ofstream of(os.str().c_str());
        //(*iter)->save(of);
        //of <<"fit="<< _f<<std::endl;
        if (_f > _max)
        {
            bestIter = iter;
            _max = _f;
        }
    }
    if (mBest!=NULL)
    {
        if (mBestFit <= _max)
        {
            mBest->decRef();
            mBest = *bestIter;
            (*bestIter)->addRef();
            mBestFit = _max;
        }
    }
    else
    {
        mBest = *bestIter;
        mBestFit = _max;
        (*bestIter)->addRef();
    }
}

void GPEvolutionGroup::_expand()
{
    _clearGroup();
    for (int i=0; i<mSize; ++i)
    {
        mGroup.push_back(mBest->copy());
    }
}

void GPEvolutionGroup::_mutate()
{
    list<IGPAutoDefFunction*>::iterator iter = mGroup.begin();
    for (;iter!=mGroup.end();iter++)
    {
        if (GPRandom::rate() < 0.1)
        {
            (*iter)->decRef();
            int n = GPRandom::mid(0,mBackup.size());
            *iter = (mBackup[n])->copy();
        }
        mSys->vMutate(*iter);
    }
}

void GPEvolutionGroup::loadBest(istream& input)
{
    GPASSERT(NULL!=mSys);
    if (NULL!=mBest)
    {
        mBest->decRef();
    }
    mBest = mSys->vCreateFunctionFromIS(input);
}

void GPEvolutionGroup::vEvolutionFunc(std::function<double(IGPAutoDefFunction*)> fit_func)
{
    if (NULL!=mBest)
    {
        mBestFit = fit_func(mBest);
    }
    /*Create the initial group*/
    _clearGroup();
    vector<IGPAutoDefFunction*> group = mSys->vCreateAllFunction(mOutput, mInput);
    GPASSERT(!group.empty());
    for (int i=0; i<group.size(); ++i)
    {
        mGroup.push_back(group[i]);
    }
    _restoreBackup();

    /*Evolution*/
    for (int i=0; i<mTime; ++i)
    {
        _best(fit_func);
        _expand();
        _mutate();
    }
}
