//
//  GPStreamFactory.cpp
//  GP
//
//  Created by jiangxiaotang on 15/7/25.
//  Copyright (c) 2015年 jiangxiaotang. All rights reserved.
//

#include <stdio.h>
#include <sstream>
#include "core/GPStreamFactory.h"
#include "GPFileStream.h"
#include "GPUserStream.h"

std::string GPStreamFactory::gPath = "";

static void _nofree(void*)
{
}
GPStreamWrap::GPStreamWrap()
{
    mFree = _nofree;
}
GPStreamWrap::~GPStreamWrap()
{
}
GPWStreamWrap::GPWStreamWrap()
{
    mFree = _nofree;
}
GPWStreamWrap::~GPWStreamWrap()
{
}


void GPStreamFactory::setParentPath(const char* path)
{
    gPath = path;
}

GPStreamWrap* GPStreamFactory::NewStream(const char* meta, MODE m)
{
    GPStreamWrap* r = NULL;
    switch(m)
    {
        case FILE:
        {
            std::ostringstream os;
            os <<gPath << meta;
            r = new GPFileStream(os.str().c_str());
            break;
        }
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
        {
            std::ostringstream os;
            os <<gPath << meta;
            r = new GPFileWStream(os.str().c_str());
            break;
        }
        case USER:
            r = new GPUserWStream(NULL);
            break;
        default:
            break;
    }
    return r;
}
