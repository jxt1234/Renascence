/******************************************************************
 Copyright 2016, Jiang Xiao-tang
 
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

#include "GPUserStream.h"
#include "utils/GPDebug.h"
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
static bool _Flush(void* meta)
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
