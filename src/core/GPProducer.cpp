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
#include "core/GPProducer.h"
#include <assert.h>
using namespace std;

bool GPProducer::initGP(AbstractGP* tree, const std::vector<int>& queue)
{
    assert(NULL!=tree);
    int cur = 0;
    tree->replacePoint(queue, cur);
    this->vSetInputNumber(tree);
    return true;
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
