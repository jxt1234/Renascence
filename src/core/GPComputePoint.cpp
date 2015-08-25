//
//  GPComputePoint.cpp
//  GP
//
//  Created by jiangxiaotang on 15/7/6.
//  Copyright (c) 2015年 jiangxiaotang. All rights reserved.
//

#include "core/GPComputePoint.h"
GPComputePoint::GPComputePoint(const GPFunctionDataBase::function* f, int n)
{
    mF = f;
    for (int i=0; i<n;++i)
    {
        mFlags.push_back(false);
        mCache.push(NULL, NULL);
    }
    mComplte = _computeCompleteStatus();
}
GPComputePoint::GPComputePoint(const GPFunctionDataBase::function* f, const std::vector<bool>& completeFlags)
{
    mF = f;
    mFlags = completeFlags;
    for (int i=0; i<completeFlags.size(); ++i)
    {
        mCache.push(NULL, NULL);
    }
    mComplte = _computeCompleteStatus();
}
GPComputePoint::~GPComputePoint()
{
    mCache.clear();
}
bool GPComputePoint::receiveSingle(GPContents* inputs, int n)
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
        dst.type->vMerge(dst.content, src.content);
    }
    mComplte = _computeCompleteStatus();
    inputs->releaseForFree();
    return mComplte;
}

bool GPComputePoint::receive(GPContents* inputs)
{
    GPASSERT(NULL!=inputs);
    GPASSERT(inputs->size() == mCache.size());
    for (int i=0; i<inputs->size(); ++i)
    {
        if (NULL == mCache.get(i))
        {
            mCache.contents[i] = inputs->contents[i];
        }
        else
        {
            auto dst = mCache[i];
            auto src = (*inputs)[i];
            GPASSERT(src.type == dst.type);
            GPASSERT(NULL!=src.type);
            GPASSERT(NULL!=src.content);
            GPASSERT(NULL!=dst.content);
            dst.type->vMerge(dst.content, src.content);
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
        if (mFlags[i])
        {
            auto c = mCache[i];
            if (!(c.type->vCheckCompleted(c.content)))
            {
                res = false;
                break;
            }
        }
    }
    return res;
}
GPContents* GPComputePoint::compute()
{
    GPASSERT(NULL!=mF);
    GPASSERT(mComplte);
    GPContents* dst = NULL;
    if (mComplte)
    {
        dst = mF->basic(&mCache);
        mCache.clearContents();
        mComplte = _computeCompleteStatus();
    }
    return dst;
}
