/******************************************************************
   Copyright 2015, Jiang Xiao-tang

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
#ifndef CORE_GPCOMPUTEPOINT_H
#define CORE_GPCOMPUTEPOINT_H
#include "head.h"
#include "core/GPFunctionDataBase.h"
#include "user/GPContents.h"
#include <vector>
#include "core/GPStatusContent.h"
class GPComputePoint:public RefCount
{
public:
    class ContentWrap:public RefCount
    {
    public:
        ContentWrap(void* content, const IStatusType* t)
        {
            GPASSERT(NULL!=content && NULL!=t);
            mT = t;
            mContent = content;
            mOwn = true;
        }
        virtual ~ContentWrap()
        {
            if (mOwn)
            {
                mT->vFree(mContent);
            }
        }
        bool needFree() const {return mOwn;}
        void releaseForFree()
        {
            mOwn = false;
        }
        void* getContent() const {return mContent;}
        const IStatusType* getType() const {return mT;}
    private:
        void* mContent;
        const IStatusType* mT;
        bool mOwn;
    };

    GPComputePoint(const GPFunctionDataBase::function* f);
    virtual ~GPComputePoint();
    /*inputs can only has one content*/
    bool receive(GPPtr<ContentWrap> inputs, int n);
    inline const GPFunctionDataBase::function* get() const {return mF;}
    const std::vector<bool>& flags() const {return mFlags;}
    int map(double* value, int n);
    inline bool completed() const {return mComplte;}
    std::vector<GPPtr<ContentWrap>> compute();
private:
    bool _computeCompleteStatus() const;
    const GPFunctionDataBase::function* mF;
    std::vector<bool> mFlags;
    std::vector<GPPtr<ContentWrap>> mCache;
    bool mComplte;
    std::vector<GPPtr<GPStatusContent> > mStatus;
};
#endif
