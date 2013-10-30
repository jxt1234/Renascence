#include "core/evolutionTreeV2.h"
#include <assert.h>
#include "utils/debug.h"
#include <iostream>

using namespace std;
float evolutionTreeV2::mStructVaryProb = 0.2;

GenerateSystem* evolutionTreeV2::mGen = NULL;
static void _limit(float& rate, float down,float up)
{
    if (rate>up) rate = up;
    if (rate<down) rate = down;
}
void evolutionTreeV2::setStructVariProb(float rate)
{
    _limit(rate, 0.0, 1.0);
    mStructVaryProb = rate;
}
void evolutionTreeV2::setGenSystem(GenerateSystem* sys)
{
    mGen = sys;
}

evolutionTreeV2::evolutionTreeV2()
{
    assert(NULL!=mGen);
    vector<int> queue = mGen->getRandSequence();
    this->replacePoint(queue, (GeneticPoint*)NULL);
    mFit = 0;
}

evolutionTreeV2::evolutionTreeV2(const evolutionTreeV2& src):GeneticProgram(src), mFit(src.mFit)
{
}

void evolutionTreeV2::operator=(const evolutionTreeV2& src)
{
    GeneticProgram::operator=(src);
    mFit = src.mFit;
}
void evolutionTreeV2::mutate_parameter()
{
    vector<int> status = getStatusId();
    for (int i=0; i<status.size(); ++i)
    {
        status_varySet(status[i]);
    }
}
void evolutionTreeV2::mutate_struct()
{
    vector<GeneticPoint*> points = searchAllPoints();
    if (points.empty())
    {
        vector<int> queue = mGen->getRandSequence();
        this->replacePoint(queue, (GeneticPoint*)NULL);
    }
    else
    {
        GeneticPoint* randPoint = points[rand()%points.size()];
        vector<int> queue = mGen->getRandSequenceWithOutput(randPoint->functionId);
        replacePoint(queue, randPoint);
    }
}
void evolutionTreeV2::mutate()
{
    assert(NULL!=mGen);
    float rate = (rand()%100)/100.0;
    if (rate < mStructVaryProb)
    {
        mutate_struct();
    }
    else
    {
        mutate_parameter();
    }
}
void evolutionTreeV2::print_f(ofstream& file)
{
}

double evolutionTreeV2::fit_comput()
{
    double* result = NULL;
    assert(NULL!=mGen);
    result = (double*)(compute(mGen).output[0].content);
    assert(NULL!=result);
    mFit = *result;
    free(result);
    return mFit;
}
