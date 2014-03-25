#include "evolution/xmlEvolutionGroup.h"
#include <assert.h>
#include <fstream>
using namespace std;
xmlEvolutionGroup::xmlEvolutionGroup(xmlGenerateSystem* sys, int time, int size):mSys(sys)
{
    assert(NULL!=sys);
    mBest = NULL;
    mStrategy = NULL;
    mTime = time;
    mSize = size;
}

xmlEvolutionGroup::~xmlEvolutionGroup()
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
}
void xmlEvolutionGroup::_clearGroup()
{
    list<IGPAutoDefFunction*>::iterator iter = mGroup.begin();
    for (; iter!=mGroup.end(); iter++)
    {
        if (NULL != *iter)
        {
            delete *iter;
        }
    }
    mGroup.clear();
}
void xmlEvolutionGroup::vSetFixInput(const GP_Input& input)
{
    class fixStrategy:public xmlEvolutionGroup::IInputStrategy
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
void xmlEvolutionGroup::vSetInputStrategy(IInputStrategy* strategy)
{
    if (NULL!=mStrategy)
    {
        mStrategy->decRef();
    }
    mStrategy = strategy;
    strategy->addRef();
}

double xmlEvolutionGroup::_fitCompute(IGPAutoDefFunction* g, IGPAutoDefFunction* fit) const
{
    ofstream f("temp.xml");
    g->save(f);
    f.close();
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

void xmlEvolutionGroup::_best(IGPAutoDefFunction* fit)
{
    int bestId = 0;
    double max = _fitCompute(*(mGroup.begin()), fit);
    list<IGPAutoDefFunction*>::iterator iter = mGroup.begin();
    list<IGPAutoDefFunction*>::iterator bestIter = mGroup.begin();
    for (iter++; iter!=mGroup.end(); iter++)
    {
        double _f = _fitCompute(*iter, fit);
        if (_f > max)
        {
            bestIter = iter;
            max = _f;
        }
    }
    if (mBest!=NULL)
    {
        if (mBestFit <= max)
        {
            mBest->decRef();
            mBest = *bestIter;
            mBestFit = max;
            mGroup.erase(bestIter);
        }
    }
    else
    {
        mBest = *bestIter;
        mBestFit = max;
        mGroup.erase(bestIter);
    }
}

void xmlEvolutionGroup::_expand()
{
    _clearGroup();
    for (int i=0; i<mTime; ++i)
    {
        mGroup.push_back(mSys->vCreateFromADF(mBest));
    }
}

void xmlEvolutionGroup::_mutate()
{
    list<IGPAutoDefFunction*>::iterator iter = mGroup.begin();
    for (;iter!=mGroup.end();iter++)
    {
        (*iter)->mutate();
    }
}

void xmlEvolutionGroup::vEvolution(IGPAutoDefFunction* fit)
{
    /*Create the initial group*/
    _clearGroup();
    vector<IGPAutoDefFunction*> group = mSys->vCreateAllFunction(mOutputId, mInputId);
    assert(!group.empty());
    for (int i=0; i<group.size(); ++i)
    {
        mGroup.push_back(group[i]);
    }
    /*Evolution*/
    for (int i=0; i<mTime; ++i)
    {
        _best(fit);
        _expand();
        _mutate();
    }
}
