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
#include "core/GPSingleTree.h"
#include <assert.h>
#include <stdlib.h>
using namespace std;
GPSinglePoint::GPSinglePoint(GPIFloatFunction* f)
{
    assert(NULL!=f);
    f->addRef();
    mF = f;
    mCache = NULL;
    if (0 < mF->size())
    {
        mCache = new GPFLOAT[mF->size()];
    }
}

GPSinglePoint::~GPSinglePoint()
{
    mF->decRef();
    delete [] mCache;
}

GPFLOAT GPSinglePoint::compute()
{
    //TODO delete assert and use force point changer here
    assert(mChildren.size() == mF->size());
    for (int i=0; i<mChildren.size(); ++i)
    {
        GPSinglePoint* p = dynamic_cast<GPSinglePoint*>(mChildren[i]);
        assert(NULL!=p);
        //GPSinglePoint* p = (GPSinglePoint*)(mChildren[i]);
        mCache[i] = p->compute();
    }
    return mF->compute(mCache);
}

GPIFloatFunction* GPIFloatFunctionSet::randomSelect()
{
    int n = rand()%mSet.size();
    return mSet[n];
}

void GPIFloatFunctionSet::clear()
{
    for (int i=0; i<mSet.size(); ++i)
    {
        mSet[i]->decRef();
    }
    mSet.clear();
}

void GPIFloatFunctionSet::add(GPIFloatFunction* func)
{
    mSet.push_back(func);
    func->addRef();
}

GPSingleTree::GPSingleTree(const GPIFloatFunctionSet& set):mSet(set)
{
}

GPSingleTree::GPSingleTree(const GPSingleTree& tree):mSet(tree.mSet)
{
}

void GPSingleTree::operator=(const GPSingleTree& tree)
{
}

GPSingleTree::~GPSingleTree()
{
    delete mRoot;
}
