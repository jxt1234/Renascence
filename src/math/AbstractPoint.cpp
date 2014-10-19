/******************************************************************
   Copyright 2013, Jiang Xiao-tang

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
#include "math/AbstractPoint.h"
#include <stdlib.h>
#include <assert.h>
#include "utils/debug.h"
using namespace std;

AbstractPoint::~AbstractPoint()
{
    for (int i=0; i<mChildren.size(); ++i)
    {
        delete mChildren[i];
    }
}

void AbstractPoint::addPoint(AbstractPoint* p)
{
    assert(NULL!=p);
    mChildren.push_back(p);
}

AbstractPoint* AbstractPoint::deepCopy(AbstractPoint* src, AbstractPoint::IPointCopy* copy)
{
    assert(NULL!=src);
    assert(NULL!=copy);
    AbstractPoint* result;
    result = copy->copy(src);
    if (NULL == result)
    {
        return result;
    }
    for (int i=0; i<(src->mChildren).size(); ++i)
    {
        AbstractPoint* child = deepCopy(src->mChildren[i], copy);
        if (NULL != child)
        {
            result->addPoint(child);
        }
    }
    return result;
}

void AbstractPoint::print(ostream& out)
{
    this->printBefore(out);
    for (int i=0; i<mChildren.size(); ++i)
    {
        mChildren[i]->print(out);
    }
    this->printAfter(out);
}

std::vector<AbstractPoint*> AbstractPoint::display()
{
    vector<AbstractPoint*> result;
    list<AbstractPoint*> cache;
    result.push_back(this);
    cache.push_back(this);
    while(!cache.empty())
    {
        AbstractPoint* p = cache.front();
        for (int i=0; i<(p->mChildren).size(); ++i)
        {
            AbstractPoint* c = p->mChildren[i];
            result.push_back(c);
            cache.push_back(c);
        }
        cache.pop_front();
    }
    return result;
}

