//
//  GPUserStream.h
//  GP
//
//  Created by jiangxiaotang on 15/8/6.
//  Copyright (c) 2015年 jiangxiaotang. All rights reserved.
//

#ifndef __GP__GPUserStream__
#define __GP__GPUserStream__

#include <stdio.h>
#include "core/GPStreamFactory.h"

class GPUserStream:public GPStreamWrap
{
public:
    GPUserStream(const char* filename);
    virtual ~GPUserStream();
};

class GPUserWStream:public GPWStreamWrap
{
public:
    GPUserWStream(const char* filename);
    virtual ~GPUserWStream();
};
#endif /* defined(__GP__GPUserStream__) */
