//
//  GPComputePoint.cpp
//  GP
//
//  Created by jiangxiaotang on 15/7/6.
//  Copyright (c) 2015年 jiangxiaotang. All rights reserved.
//

#include "core/GPComputePoint.h"
GPComputePoint::GPComputePoint(computeFunction f, int n)
{
    mF = f;
    for (int i=0; i<n;++i)
    {
        mFlags.push_back(false);
        mCache.push(NULL, NULL);
    }
    mComplte = false;
}
GPComputePoint::GPComputePoint(computeFunction f, const std::vector<bool>& completeFlags)
{
    mF = f;
    mFlags = completeFlags;
    for (int i=0; i<completeFlags.size(); ++i)
    {
        mCache.push(NULL, NULL);
    }
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
    mComplte = true;
    for (int i=0; i<mFlags.size(); ++i)
    {
        if (mFlags[i])
        {
            auto c = mCache[i];
            if (!(c.type->vCheckCompleted(c.content)))
            {
                mComplte = false;
                break;
            }
        }
    }
    return mComplte;
}
GPContents* GPComputePoint::vCompute()
{
    GPASSERT(NULL!=mF);
    GPContents* dst = NULL;
    if (mComplte)
    {
        dst = mF(&mCache);
        mCache.clearContents();
        mComplte = false;
    }
    return dst;
}
