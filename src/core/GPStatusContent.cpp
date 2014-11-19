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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "utils/debug.h"
#include "utils/AutoStorage.h"
#include "utils/GPRandom.h"
#include "core/GPStatusContent.h"
using namespace std;

GPStatusContent::GPStatusContent(const IStatusType* t)
{
    mType = t;
    mNumber = mType->vMap(NULL, NULL);
    mValues = new double[mNumber];
    for (int i=0; i<mNumber; ++i)
    {
        mValues[i] = 0.5;
    }
    mContent = NULL;//Important, Let mType alloc content when vMap
    mType->vMap(&mContent, mValues);
}

GPStatusContent::GPStatusContent(const GPStatusContent& c)
{
    mType = c.mType;
    mNumber = c.mNumber;
    mValues = new double[mNumber];
    ::memcpy(mValues, c.mValues, sizeof(double)*mNumber);
    mContent = NULL;//Important, Let mType alloc content when vMap
    mType->vMap(&mContent, mValues);
}

void GPStatusContent::operator=(const GPStatusContent& c)
{
    if (mType != c.mType)
    {
        delete [] mValues;
        mValues = NULL;
        mType->vFree(mContent);
        mContent = NULL;
        mType = c.mType;
        mNumber = c.mNumber;
        mValues = new double[mNumber];
    }
    ::memcpy(mValues, c.mValues, mNumber*sizeof(double));
    mType->vMap(&mContent, mValues);
}

GPStatusContent::~GPStatusContent()
{
    mType->vFree(mContent);
    delete [] mValues;
}

void GPStatusContent::print(std::ostream& out) const
{
    for (int i=0; i<mNumber; ++i)
    {
        out << mValues[i] <<" ";
    }
}

void GPStatusContent::setValue(double* value, int n)
{
    GPASSERT(n == mNumber);
    ::memcpy(mValues, value, n*sizeof(double));
    mType->vMap(&mContent, mValues);
}

void GPStatusContent::mutate()
{
    for (int i=0; i<mNumber; ++i)
    {
        float r = (GPRandom::rate()-0.5)/10.0;
        double newv = mValues[i]+r;
        newv = std::max(newv, 1.0);
        newv = std::min(newv, 0.0);
        mValues[i] = newv;
    }
}

