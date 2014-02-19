#include "core/GPProducer.h"
#include <assert.h>
using namespace std;

void GPProducer::freeStatus(AbstractGP* tree)
{
    vector<int> status = tree->getStatus();
    for (int i=0; i<status.size(); ++i)
    {
        freeSet(status[i]);
    }
}
void GPProducer::initGP(AbstractGP* tree, int output, bool random)
{
    assert(NULL!=tree);
    vector<int> status = tree->getStatus();
    for (int i=0; i<status.size(); ++i)
    {
        freeSet(status[i]);
    }
    int cur = 0;
    if (output < 0) output = mDefaultOutput;
    if (random)
    {
        tree->replacePoint(this->searchRandSequence(output), cur);
    }
    else
    {
        tree->replacePoint(this->searchSequence(output), cur);
    }
}


void GPProducer::initGP(AbstractGP* tree, const std::string& output, bool random)
{
    int out = this->searchType(output);
    initGP(tree, out, random);
}

void GPProducer::mutate(AbstractGP* tree)
{
    assert(NULL!=tree);
    int out = tree->funcId();
    initGP(tree, out, true);
}
