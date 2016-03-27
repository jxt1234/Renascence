//
//  GPComputePoint.cpp
//  GP
//
//  Created by jiangxiaotang on 15/7/6.
//  Copyright (c) 2015年 jiangxiaotang. All rights reserved.
//

#include "backend/GPComputePoint.h"
GPComputePoint::GPComputePoint(const GPFunction* f)
{
    mF = f;
    mFlags = f->inputNeedComplete;
    if (mFlags.empty())
    {
        for (int i=0; i<f->inputType.size(); ++i)
        {
            mFlags.push_back(false);
        }
    }
    GPASSERT(mFlags.size() == f->inputType.size());
    for (int i=0; i<mFlags.size(); ++i)
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
    mCache.clear();
}
bool GPComputePoint::receive(GPPtr<ContentWrap> inputs, int n)
{
    GPASSERT(NULL!=inputs.get());
    GPASSERT(0<=n && n<mCache.size());
    if (mCache[n].get()!=NULL)
    {
        auto dst = mCache[n];
        GPASSERT(inputs->type() == dst->type());
        if (dst->content() == inputs->content())
        {
            return false;
        }
        void* merge = dst->type()->vMerge(dst->content(), inputs->content());
        if (NULL == merge)
        {
            mCache[n] = inputs;
        }
        else
        {
            GPPtr<ContentWrap> wrap = new ContentWrap(merge, dst->type());
            mCache[n] = wrap;
        }
    }
    else
    {
        auto type = inputs->type();
        void* merge = NULL;
        merge = type->vMerge(NULL, inputs->content());
        if (NULL == merge)
        {
            mCache[n] = inputs;
        }
        else
        {
            mCache[n] = new ContentWrap(merge, type);
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
        auto c = mCache[i];
        if (NULL == c.get())
        {
            res = false;
            break;
        }
        if (mFlags[i])
        {
            if (!(c->type()->vCheckCompleted(c->content())))
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
            if (!mFlags[i])
            {
                mCache[i] = NULL;
            }
        }
        mComplte = _computeCompleteStatus();
    }
    /*FIXME*/
    GPASSERT(NULL!=dst);
    return dst;
}
