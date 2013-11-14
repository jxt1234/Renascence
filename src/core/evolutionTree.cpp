#include "core/evolutionTree.h"
#include <assert.h>
#include <stdlib.h>
GenerateSystem* evolutionTree::mGen = NULL;

double evolutionTree::fit_comput()
{
    _reset();
    compute(mGen);
    return get_fit();
}

evolutionTree::evolutionTree()
{
    assert(mGen!=NULL);
    int cur = 0;
    replacePoint(mGen->getRandSequence(), cur);
}

double evolutionTree::get_fit()
{
    double* result;
    result = (double*)((mSave->output[0]).content);
    return *result;
}

void evolutionTree::mutate()
{
    assert(NULL!=mGen);
    /*Little prob to totally */
    if (mStatus!=-1)
    {
    }
}
