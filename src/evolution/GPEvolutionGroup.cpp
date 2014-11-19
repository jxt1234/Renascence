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
    mStrategy = NULL;
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
    if (NULL!=mStrategy)
    {
        mStrategy->decRef();
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
void GPEvolutionGroup::vSetFixInput(const GP_Input& input)
{
    class fixStrategy:public GPEvolutionGroup::IInputStrategy
    {
        public:
            fixStrategy(const GP_Input& input):mInput(input){}
            ~fixStrategy(){}
            virtual GP_Input vCreate(IGPAutoDefFunction* f) const {return mInput;}
        private:
            GP_Input mInput;
    };
    if (NULL!=mStrategy)
    {
        mStrategy->decRef();
    }
    mStrategy = new fixStrategy(input);
}
void GPEvolutionGroup::vSetInputStrategy(IInputStrategy* strategy)
{
    if (NULL!=mStrategy)
    {
        mStrategy->decRef();
    }
    mStrategy = strategy;
    strategy->addRef();
}

void GPEvolutionGroup::optimizeParameters(IGPAutoDefFunction* g, IGPAutoDefFunction* fit) const
{
    class computer:public IGPOptimizor::IComputer
    {
        public:
            computer(GP_Input& input, IGPAutoDefFunction* fit, IGPAutoDefFunction* g):I(input), F(fit), G(g){}
            virtual PFLOAT run(GPPtr<GPParameter> p)
            {
                G->vMap(p);
                GP_Output out1 = G->run(I);
                GP_Input input;
                GP_Output_collect(input, out1);
                GP_Output out2 = F->run(input);
                double* f = (double*)out1[0];
                out1.clear();
                out2.clear();
                PFLOAT result = *f;
                return result;
            }
        private:
            GP_Input& I;
            IGPAutoDefFunction* F;
            IGPAutoDefFunction* G;
    };

    GPPtr<GPParameter> pa;
    int number = g->vMap(pa);//Get the number of map values
    std::istringstream parameter("40");
    GPPtr<IGPOptimizor> optimizor = GPOptimizorFactory::create(GPOptimizorFactory::PSO_SEARCH, &parameter);
    GP_Input input = mStrategy->vCreate(g);
    GPPtr<IGPOptimizor::IComputer> c = new computer(input, fit, g);
    pa = optimizor->vFindBest(number, c);
    g->vMap(pa);
}

double GPEvolutionGroup::_fitCompute(IGPAutoDefFunction* g, IGPAutoDefFunction* fit) const
{
/*Debug */
#if DEBUG_EVOLUTION
    static int num =0;
    ostringstream is;
    is << "temp/"<<num<<"_"<<g<<".xml";
    num++;
    ofstream f(is.str().c_str());
    g->save(f);
    f.close();
#endif
    //optimizeParameters(g, fit);
    GP_Output out = g->run(mStrategy->vCreate(g));
    GP_Input inp;
    GP_Output_collect(inp, out);
    GP_Output fitValue = fit->run(inp);
    double* fitV = (double*)((fitValue.output)[0].content);
    double res = *fitV;
    GP_Output_clear(out);
    GP_Output_clear(fitValue);
    return res;
}

void GPEvolutionGroup::_best(IGPAutoDefFunction* fit)
{
    int bestId = 0;
    double _max = _fitCompute(*(mGroup.begin()), fit);
    list<IGPAutoDefFunction*>::iterator iter = mGroup.begin();
    list<IGPAutoDefFunction*>::iterator bestIter = mGroup.begin();
    for (iter++; iter!=mGroup.end(); iter++)
    {
        double _f = _fitCompute(*iter, fit);
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

void GPEvolutionGroup::vEvolution(IGPAutoDefFunction* fit)
{
    if (NULL!=mBest)
    {
        mBestFit = _fitCompute(mBest, fit);
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
        _best(fit);
        _expand();
        _mutate();
    }
}
