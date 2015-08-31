//
//  GPComputePoint.cpp
//  GP
//
//  Created by jiangxiaotang on 15/7/6.
//  Copyright (c) 2015年 jiangxiaotang. All rights reserved.
//

#include "core/GPComputePoint.h"
GPComputePoint::GPComputePoint(const GPFunctionDataBase::function* f, const std::vector<bool>& completeFlags)
{
    mF = f;
    mFlags = completeFlags;
    for (int i=0; i<completeFlags.size(); ++i)
    {
        mCache.push(NULL, NULL);
    }
    mComplte = _computeCompleteStatus();
    for (auto s : f->statusType)
    {
        mStatus.push_back(new GPStatusContent(s));
    }
}
GPComputePoint::~GPComputePoint()
{
    mCache.clear();
}
bool GPComputePoint::receive(GPContents* inputs, int n)
{
    GPASSERT(NULL!=inputs);
    GPASSERT(1 == inputs->size());
    GPASSERT(0<=n && n<mCache.size());
    if (NULL == mCache.get(n))
    {
        mCache.contents[n] = inputs->contents[0];
    }
    else
    {
        auto dst = mCache[n];
        auto src = (*inputs)[0];
        GPASSERT(src.type == dst.type);
        GPASSERT(NULL!=src.type);
        GPASSERT(NULL!=src.content);
        GPASSERT(NULL!=dst.content);
        if (mFlags[n])
        {
            dst.type->vMerge(dst.content, src.content);
        }
        else
        {
            /*Refresh*/
            dst.type->vFree(dst.content);
            mCache.contents[n].content = src.content;
        }
    }
    mComplte = _computeCompleteStatus();
    inputs->releaseForFree();
    return mComplte;
}

bool GPComputePoint::_computeCompleteStatus() const
{
    bool res = true;
    for (int i=0; i<mFlags.size(); ++i)
    {
        auto c = mCache[i];
        if (NULL == c.content)
        {
            res = false;
            break;
        }
        if (mFlags[i])
        {
            if (!(c.type->vCheckCompleted(c.content)))
            {
                res = false;
                break;
            }
        }
    }
    return res;
}
int GPComputePoint::map(double* value, int n)
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
    if (mComplte)
    {
        GPContents inputs;
        for (auto c : mCache.contents)
        {
            inputs.push(c);
        }
        for (auto s : mStatus)
        {
            inputs.push(s->content(), s->type());
        }
        dst = mF->basic(&inputs);
        mCache.clearContents();
        mComplte = _computeCompleteStatus();
    }
    return dst;
}
