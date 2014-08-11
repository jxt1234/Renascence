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
#include "core/status.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils/debug.h"
#include <execinfo.h>
#include <sstream>
#include <iostream>
#include <assert.h>
using namespace std;

GPStatusContent::GPStatusContent(const IStatusType* t)
{
    mType = t;
    mContent = t->Alloc();
}
GPStatusContent::GPStatusContent(const IStatusType* t, istream& is)
{
    mType = t;
    mContent = t->load(is);
}

GPStatusContent::GPStatusContent(const GPStatusContent& c)
{
    mType = c.mType;
    mContent = mType->Alloc();
    mType->copy(mContent, c.mContent);
}

void GPStatusContent::operator=(const GPStatusContent& c)
{
    if (mType != c.mType)
    {
        mType = c.mType;
        mType->Free(mContent);
        mContent = mType->Alloc();
    }
    mType->copy(mContent, c.mContent);
}

GPStatusContent::~GPStatusContent()
{
    mType->Free(mContent);
}

void GPStatusContent::mutate()
{
    mType->mutate(mContent);
}

void GPStatusContent::mapValue(double value)
{
    mType->mapValue(mContent, value);
}
