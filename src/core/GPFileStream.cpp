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

#include "GPFileStream.h"
#include <stdio.h>
#include "utils/GPDebug.h"

static size_t _Read(void* meta, void* buffer, size_t size)
{
    FILE* f = (FILE*)meta;
    if (NULL == buffer)
    {
        long curr = ftell(f);
        if (-1 == curr)
        {
            return 0;
        }
        int err = fseek((FILE*)f, (long)size, SEEK_CUR);
        if (err != 0)
        {
            return 0;
        }
        return size;
    }
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
    GPASSERT(NULL!=filename);
    FILE* f = fopen(filename, "rb");
    if (NULL == f)
    {
        FUNC_PRINT_ALL(filename, s);
    }
    GPASSERT(NULL!=f);//TODO
    mF = f;
}

GPFileStream::~GPFileStream()
{
    fclose(mF);
}
size_t GPFileStream::vRead(void* buffer, size_t size)
{
    return _Read(mF, buffer, size);
}
bool GPFileStream::vIsEnd() const
{
    return _IsEnd(mF);
}
bool GPFileStream::vRewind()
{
    return 0 == fseek(mF, 0L, SEEK_SET);
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
    mF = f;
}

size_t GPFileWStream::vWrite(const void* buffer, size_t size)
{
    return _Write(mF, buffer, size);
}

bool GPFileWStream::vFlush()
{
    return _Flush(mF);
}


GPFileWStream::~GPFileWStream()
{
    fclose(mF);
}
