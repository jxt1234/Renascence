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
size_t GPUserStream::vRead(void* buffer, size_t size)
{
    //TODO
    return 0;
}
bool GPUserStream::vIsEnd() const
{
    return true;
}
bool GPUserStream::vRewind()
{
    return false;
}

GPUserStream::GPUserStream(const char* filename)
{
}

GPUserStream::~GPUserStream()
{
}

size_t GPUserWStream::vWrite(const void* buffer, size_t size)
{
    cout.write((char*)buffer, size);
    return size;
}
bool GPUserWStream::vFlush()
{
    cout.flush();
    return true;
}


GPUserWStream::GPUserWStream(const char* name)
{
}

GPUserWStream::~GPUserWStream()
{
}
