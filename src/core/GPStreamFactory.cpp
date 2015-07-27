//
//  GPStreamFactory.cpp
//  GP
//
//  Created by jiangxiaotang on 15/7/25.
//  Copyright (c) 2015年 jiangxiaotang. All rights reserved.
//

#include <stdio.h>
#include "core/GPStreamFactory.h"
#include "GPFileStream.h"

GPStreamWrap* GPStreamFactory::NewStream(const char* meta, MODE m)
{
    GPStreamWrap* r = NULL;
    switch(m)
    {
        case FILE:
            r = new GPFileStream(meta);
            break;
        default:
            break;
    }
    return r;
}
GPWStreamWrap* GPStreamFactory::NewWStream(const char* meta, MODE m)
{
    GPWStreamWrap* r = NULL;
    switch(m)
    {
        case FILE:
            r = new GPFileWStream(meta);
            break;
        default:
            break;
    }
    return r;
}
