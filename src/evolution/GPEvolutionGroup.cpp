#include "evolution/GPEvolutionGroup.h"
#include <assert.h>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;
GPEvolutionGroup::GPEvolutionGroup(GPProducer* sys, int time, int size):mSys(sys)
{
    assert(NULL!=sys);
    mBest = NULL;
    mStrategy = NULL;
    mTime = time;
    mSize = size;
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
        mBackup.push_back(mSys->vCopyADF(*iter));
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
        //FUNC_PRINT_ALL(_f, f);
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
        mGroup.push_back(mSys->vCopyADF(mBest));
    }
}

void GPEvolutionGroup::_mutate()
{
    list<IGPAutoDefFunction*>::iterator iter = mGroup.begin();
    for (;iter!=mGroup.end();iter++)
    {
        int r = rand()%10;
        if (r < 1)
        {
            (*iter)->decRef();
            int n = rand()%mBackup.size();
            *iter = mSys->vCopyADF(mBackup[n]);
        }
        mSys->vMutate(*iter);
    }
}

void GPEvolutionGroup::loadBest(istream& input)
{
    assert(NULL!=mSys);
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
    assert(!group.empty());
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
