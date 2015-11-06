//
//  GPComputePoint.cpp
//  GP
//
//  Created by jiangxiaotang on 15/7/6.
//  Copyright (c) 2015年 jiangxiaotang. All rights reserved.
//

#include "core/GPComputePoint.h"
GPComputePoint::GPComputePoint(const GPFunctionDataBase::function* f)
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
    if (mCache[n].get()!=NULL && mFlags[n])
    {
        auto dst = mCache[n];
        GPASSERT(inputs->getType() == dst->getType());
        dst->getType()->vMerge(dst->getContent(), inputs->getContent());
    }
    else
    {
        mCache[n] = inputs;
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
            if (!(c->getType()->vCheckCompleted(c->getContent())))
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

std::vector<GPPtr<GPComputePoint::ContentWrap>> GPComputePoint::compute()
{
    GPASSERT(NULL!=mF);
    GPASSERT(mComplte);
    GPContents* dst = NULL;
    if (mComplte)
    {
        GPContents inputs;
        for (auto c : mCache)
        {
            inputs.push(c->getContent(), c->getType());
        }
        for (auto s : mStatus)
        {
            inputs.push(s->content(), s->type());
        }
        dst = mF->basic(&inputs);
        for (int i=0; i<mCache.size(); ++i)
        {
            mCache[i] = NULL;
        }
        mComplte = _computeCompleteStatus();
    }
    /*FIXME*/
    GPASSERT(NULL!=dst);
    std::vector<GPPtr<ContentWrap>> result;
    for (auto c : dst->contents)
    {
        result.push_back(new ContentWrap(c.content, c.type));
    }
    delete dst;
    return result;
}
