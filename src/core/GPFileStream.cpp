//
//  GPFileStream.cpp
//  GP
//
//  Created by jiangxiaotang on 15/7/25.
//  Copyright (c) 2015年 jiangxiaotang. All rights reserved.
//

#include "GPFileStream.h"
#include <stdio.h>
#include "utils/debug.h"

static size_t _Read(void* meta, void* buffer, size_t size)
{
    FILE* f = (FILE*)meta;
    auto s = fread(buffer, 1, size, f);
    return s;
}

static bool _IsEnd(void* meta)
{
    FILE* f = (FILE*)meta;
    return feof(f)!=0;
}

GPFileStream::GPFileStream(const char* filename)
{
    FILE* f = fopen(filename, "rb");
    GPASSERT(NULL!=f);
    mMetaData = (void*)f;
    mRead = _Read;
    mIsEnd = _IsEnd;
}

GPFileStream::~GPFileStream()
{
    fclose((FILE*)mMetaData);
}

static size_t _Write(void* meta, const void* buffer, size_t size)
{
    FILE* f = (FILE*)meta;
    return fwrite(buffer, 1, size, f);
}
static bool _Flush(void* meta)
{
    FILE* f = (FILE*)meta;
    return 0 == fflush(f);
}


GPFileWStream::GPFileWStream(const char* name)
{
    FILE* f = fopen(name, "wb");
    GPASSERT(NULL!=f);
    mMetaData = (void*)f;
    mWrite = _Write;
    mFlush = _Flush;
}

GPFileWStream::~GPFileWStream()
{
    fclose((FILE*)mMetaData);
}