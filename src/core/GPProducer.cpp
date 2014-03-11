#include "core/GPProducer.h"
#include <assert.h>
using namespace std;

bool GPProducer::initGP(AbstractGP* tree, const std::vector<int>& queue)
{
    assert(NULL!=tree);
    int cur = 0;
    tree->replacePoint(queue, cur);
    return true;
}

void GPProducer::vDestroyFunction(IGPAutoDefFunction* f)
{
    if (NULL!=f)
    {
        delete f;
    }
}
AbstractGP* GPProducer::loadGP(IGPAutoDefFunction* f)
{
    assert(NULL!=f);
    AbstractGP* gp = dynamic_cast<AbstractGP*>(getBase(f));
    return gp;
}
void GPProducer::freeStatus(AbstractGP* tree)
{
    vector<int> status = tree->getStatus();
    for (int i=0; i<status.size(); ++i)
    {
        freeSet(status[i]);
    }
}
bool GPProducer::initGP(AbstractGP* tree, int output, bool random)
{
    assert(NULL!=tree);
    if (output < 0) output = mDefaultOutput;
    vector<int> queue;
    if (random)
    {
        queue = this->searchRandSequence(output);
    }
    else
    {
        queue = this->searchSequence(output);
    }
    if (queue.empty()) return false;
    /*Replace*/
    vector<int> status = tree->getStatus();
    for (int i=0; i<status.size(); ++i)
    {
        freeSet(status[i]);
    }
    int cur = 0;
    tree->replacePoint(queue, cur);
    return true;
}


bool GPProducer::initGP(AbstractGP* tree, const std::string& output, bool random)
{
    vector<int> out = this->searchType(output);
    if (out.empty()) return false;
    if (random)
    {
        int outputFunc = rand()%out.size();
        return initGP(tree, outputFunc, true);
    }
    /*TODO For random scene, wide search a available sequence*/
    for (int i=0; i<out.size(); ++i)
    {
        bool res = initGP(tree, out[i], random);
        if (res)
        {
            return true;
        }
    }
    return false;
}

void GPProducer::mutate(AbstractGP* tree)
{
    assert(NULL!=tree);
    int out = tree->funcId();
    initGP(tree, out, true);
}
