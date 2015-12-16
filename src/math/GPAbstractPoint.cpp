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
#include "math/GPAbstractPoint.h"
#include <stdlib.h>
#include <utils/GPDebug.h>
#include "utils/GPDebug.h"
using namespace std;

GPAbstractPoint::~GPAbstractPoint()
{
    for (int i=0; i<mChildren.size(); ++i)
    {
        mChildren[i]->decRef();
    }
}

int GPAbstractPoint::_posOfChild(GPAbstractPoint* p) const
{
    int pos = -1;
    for (int i=0; i<mChildren.size(); ++i)
    {
        if (p == mChildren[i])
        {
            pos = i;
            break;
        }
    }
    return pos;
}


bool GPAbstractPoint::replace(GPAbstractPoint* oldPoint, GPAbstractPoint* newPoint)
{
    GPASSERT(NULL!=oldPoint);
    GPASSERT(NULL!=newPoint);
    int pos = _posOfChild(oldPoint);
    if (pos >=0)
    {
        mChildren[pos]->decRef();
        mChildren[pos]=newPoint;
        newPoint->addRef();
        return true;
    }
    for (auto c : mChildren)
    {
        if (c->replace(oldPoint, newPoint))
        {
            return true;
        }
    }
    return false;
}


void GPAbstractPoint::addPoint(GPAbstractPoint* p)
{
    GPASSERT(NULL!=p);
    mChildren.push_back(p);
}

GPAbstractPoint* GPAbstractPoint::deepCopy(GPAbstractPoint* src, GPAbstractPoint::IPointCopy* copy)
{
    GPASSERT(NULL!=src);
    GPASSERT(NULL!=copy);
    GPAbstractPoint* result;
    bool needcopyChild = true;
    result = copy->copy(src, needcopyChild);
    if (NULL == result || !needcopyChild)
    {
        return result;
    }
    for (int i=0; i<(src->mChildren).size(); ++i)
    {
        GPAbstractPoint* child = deepCopy(src->mChildren[i], copy);
        if (NULL != child)
        {
            result->addPoint(child);
        }
    }
    return result;
}

std::vector<const GPAbstractPoint*> GPAbstractPoint::display() const
{
    vector<const GPAbstractPoint*> result;
    list<const GPAbstractPoint*> cache;
    result.push_back(this);
    cache.push_back(this);
    while(!cache.empty())
    {
        const GPAbstractPoint* p = cache.front();
        for (int i=0; i<(p->mChildren).size(); ++i)
        {
            const GPAbstractPoint* c = p->mChildren[i];
            result.push_back(c);
            cache.push_back(c);
        }
        cache.pop_front();
    }
    return result;
}


void GPAbstractPoint::clearChildren()
{
    for (auto c:mChildren)
    {
        delete c;
    }
    mChildren.clear();
}


bool GPAbstractPoint::isChildOf(const GPAbstractPoint* parent) const
{
    GPASSERT(NULL!=parent);
    for (auto p : parent->display())
    {
        if (this == p)
        {
            return true;
        }
    }
    return false;
}
