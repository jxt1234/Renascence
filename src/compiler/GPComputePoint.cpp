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
//
//  GPComputePoint.cpp
//  GP
//
//  Created by jiangxiaotang on 15/7/6.
//  Copyright (c) 2015年 jiangxiaotang. All rights reserved.
//

#include "compiler/GPComputePoint.h"
GPComputePoint::GPComputePoint(const GPFunction* f)
{
    mF = f;
    for (int i=0; i<f->inputType.size(); ++i)
    {
        mCache.push_back(NULL);
    }
    mComplte = _computeCompleteStatus();
    for (auto s : f->statusType)
    {
        mStatus.push_back(new GPStatusContent(s));
    }
}
GPComputePoint::~GPComputePoint()
{
}
bool GPComputePoint::receive(GPPtr<ContentWrap> inputs, int n)
{
    GPASSERT(NULL!=inputs.get());
    GPASSERT(0<=n && n<mCache.size());
    GPASSERT(NULL == mCache[n].get());
    mCache[n] = inputs;
    mComplte = _computeCompleteStatus();
    return mComplte;
}

bool GPComputePoint::_computeCompleteStatus() const
{
    bool res = true;
    for (int i=0; i<mCache.size(); ++i)
    {
        auto c = mCache[i];
        if (NULL == c.get())
        {
            res = false;
            break;
        }
    }
    return res;
}
int GPComputePoint::map(const double* value, int n)
{
    if (NULL == value)
    {
        int sum = 0;
        for (auto s : mStatus)
        {
            sum += s->size();
        }
        return sum;
    }
    int offset = 0;
    for (auto s: mStatus)
    {
        s->setValue(value+offset, n);
        offset+= s->size();
        n-=s->size();
    }
    return offset;
}

GPContents* GPComputePoint::compute()
{
    GPASSERT(NULL!=mF);
    GPASSERT(mComplte);
    GPContents* dst = NULL;
    GPContents inputs;
    for (auto c : mCache)
    {
        inputs.push(c->content(), c->type(), false);
    }
    for (auto s : mStatus)
    {
        inputs.push(s->content(), s->type(), false);
    }
    dst = mF->basic(&inputs);
    for (int i=0; i<mCache.size(); ++i)
    {
        mCache[i] = NULL;
    }
    mComplte = false;
    /*FIXME*/
    GPASSERT(NULL!=dst);
    return dst;
}
