/******************************************************************
 Copyright 2016, Jiang Xiao-tang
 
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

#include "utils/GPDebug.h"
#include "core/GPParameter.h"
#include <string.h>
GPParameter::GPParameter(int n, const GPFLOAT* c)
{
    GPASSERT(n>=0);
    mNum = n;
    if (0 == n)
    {
        mContent = NULL;
        return;
    }
    mContent = new GPFLOAT[n];
    if (NULL!=c)
    {
        for (int i=0; i<n; ++i)
        {
            GPFLOAT _c = c[i];
            GPASSERT(_c>=0.0 && _c<=1.0);
        }
        ::memcpy(mContent, c, sizeof(GPFLOAT)*n);
    }
    else
    {
        for (int i=0; i<n; ++i)
        {
            mContent[i] = 0.5;
        }
    }
}

GPParameter::~GPParameter()
{
    if (NULL != mContent)
    {
        delete [] mContent;
    }
}

void GPParameter::clear(GPFLOAT f)
{
    for (int i=0; i<mNum; ++i)
    {
        mContent[i] = f;
    }
}

void GPParameter::makeValid()
{
    for (int i=0; i<mNum; ++i)
    {
        auto f = mContent[i];
        if (f < 0.0)
        {
            f = 0.0;
        }
        if (f>1.0)
        {
            f = 1.0;
        }
        mContent[i] = f;
    }
}
GPParameter* GPParameter::copy() const
{
    return new GPParameter(mNum, mContent);
}
