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
bool GPComputePoint::vReceive(GPContents* inputs)
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
GPContents* GPComputePoint::vCompute()
{
    GPASSERT(NULL!=mF);
    GPContents* dst = NULL;
    if (mComplte)
    {
        dst = mF->basic(&mCache);
        mCache.clearContents();
        mComplte = _computeCompleteStatus();
    }
    return dst;
}
