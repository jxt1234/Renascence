//
//  GPMemoryStream.h
//  GP
//
//  Created by jiangxiaotang on 15/7/27.
//  Copyright (c) 2015年 jiangxiaotang. All rights reserved.
//

#ifndef __GP__GPMemoryStream__
#define __GP__GPMemoryStream__

#include <stdio.h>
#include "core/GPStreamFactory.h"
class GPMemoryStream:public GPStreamWrap
{
public:
    GPMemoryStream(const char* buffer, size_t length);
    virtual ~GPMemoryStream();
private:
    const char* mSrc;
    size_t mLength;
};

class GPMemoryWStream:public GPWStreamWrap
{
public:
    GPMemoryWStream(char* buffer, size_t length);
    virtual ~GPMemoryWStream();
private:
    char* mDst;
};
#endif /* defined(__GP__GPMemoryStream__) */
