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
#include "GPStlStream.h"

GPStlWStream::GPStlWStream(std::ostream& outputStream):mStream(outputStream)
{
}
size_t GPStlWStream::vWrite(const void* buffer, size_t size)
{
    mStream.write((const char*)buffer, size);
    return size;
}
bool GPStlWStream::vFlush()
{
    mStream.flush();
    return true;
}

GPStlWStream::~GPStlWStream()
{
    mStream.flush();
}

size_t GPStlStream::vRead(void* buffer, size_t size)
{
    mStream.read((char*)buffer, size);
    auto count = mStream.gcount();
    if (count <= 0)
    {
        mEnd = true;
    }
    return count;
}
bool GPStlStream::vIsEnd() const
{
    return mEnd;
}
bool GPStlStream::vRewind()
{
    mStream.seekg(0, mStream.beg);
    return true;
}

