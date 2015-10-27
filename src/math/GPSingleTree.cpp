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
#include "math/GPSingleTree.h"
#include <utils/GPDebug.h>
#include <stdlib.h>
using namespace std;
GPSinglePoint::GPSinglePoint(GPIFloatFunction* f)
{
    GPASSERT(NULL!=f);
    f->addRef();
    mF = f;
    mCache = NULL;
    if (0 < mF->size())
    {
        mCache = new GPFLOAT[mF->size()];
        GPASSERT(NULL!=mCache);
    }
}

GPSinglePoint::~GPSinglePoint()
{
    mF->decRef();
    delete [] mCache;
}

GPFLOAT GPSinglePoint::compute()
{
    GPASSERT(mChildren.size() == mF->size());
    for (int i=0; i<mChildren.size(); ++i)
    {
        GPSinglePoint* p = (GPSinglePoint*)(mChildren[i]);
        mCache[i] = p->compute();
    }
    return mF->compute(mCache);
}

GPAbstractPoint* GPSinglePoint::GPSinglePointCopy::copy(GPAbstractPoint* src)
{
    GPSinglePoint* s = (GPSinglePoint*)src;
    GPSinglePoint* res = new GPSinglePoint(s->mF);
    return (GPAbstractPoint*)res;
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
    mRoot = NULL;
}

GPSingleTree::GPSingleTree(const GPSingleTree& tree):mSet(tree.mSet)
{
    if (NULL !=tree.mRoot)
    {
        GPSinglePoint::GPSinglePointCopy c;
        mRoot = (GPSinglePoint*)GPAbstractPoint::deepCopy(tree.mRoot, &c);
    }
}

void GPSingleTree::operator=(const GPSingleTree& tree)
{
    GPASSERT(&mSet == &(tree.mSet));
    if (NULL!=mRoot)
    {
        delete mRoot;
        mRoot = NULL;
    }
    if (NULL !=tree.mRoot)
    {
        GPSinglePoint::GPSinglePointCopy c;
        mRoot = (GPSinglePoint*)GPAbstractPoint::deepCopy(tree.mRoot, &c);
    }
}

GPSingleTree::~GPSingleTree()
{
    delete mRoot;
}
int GPSingleTree::len()
{
    if (NULL == mRoot) return 0;
    vector<GPAbstractPoint*> list = mRoot->display();
    return list.size();
}
