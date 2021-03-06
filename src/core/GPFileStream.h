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

#include "core/GPStreamFactory.h"
#include <stdio.h>


class GPFileStream:public GPStream
{
public:
    GPFileStream(const char* filename);
    virtual size_t vRead(void* buffer, size_t size);
    virtual bool vIsEnd() const;
    virtual bool vRewind();
    virtual ~GPFileStream();
    
    static bool exist(const char* filename);
private:
    FILE* mF;
};

class GPFileWStream:public GPWStream
{
public:
    GPFileWStream(const char* filename);
    virtual ~GPFileWStream();
    virtual size_t vWrite(const void* buffer, size_t size);
    virtual bool vFlush();
private:
    FILE* mF;
};
