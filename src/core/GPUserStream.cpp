//
//  GPUserStream.cpp
//  GP
//
//  Created by jiangxiaotang on 15/8/6.
//  Copyright (c) 2015年 jiangxiaotang. All rights reserved.
//

#include "GPUserStream.h"
#include "utils/debug.h"
#include <iostream>
using namespace std;
static size_t gMagic = 0;
static size_t _Read(void* meta, void* buffer, size_t size)
{
    return 0;//TODO
}

static bool _IsEnd(void* meta)
{
    return true;
}

GPUserStream::GPUserStream(const char* filename)
{
    mMetaData = (void*)(&gMagic);
    mRead = _Read;
    mIsEnd = _IsEnd;
}

GPUserStream::~GPUserStream()
{
}

static size_t _Write(void* meta, const void* buffer, size_t size)
{
    cout.write((char*)buffer, size);
    return 0;
}
bool _Flush(void* meta)
{
    cout.flush();
    return true;
}


GPUserWStream::GPUserWStream(const char* name)
{
    /*Avoid invalid*/
    mMetaData = (void*)(&gMagic);
    mWrite = _Write;
    mFlush = _Flush;
}

GPUserWStream::~GPUserWStream()
{
}