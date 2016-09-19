/******************************************************************
   Copyright 2015, Jiang Xiao-tang

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
#ifndef USER_GPSTREAM_H
#define USER_GPSTREAM_H
#include <stdlib.h>
#include "GPRefCount.h"
class GPStream : public GPRefCount
{
public:
    virtual ~GPStream() {}
    /*If the buffer is NULL, it means skip size bytes, return the fact bytes it read*/
    virtual size_t vRead(void* buffer, size_t size) = 0;
    /*Return true if the stream has moved to end*/
    virtual bool vIsEnd() const = 0;
    /*Reset the stream, return false if the stream can't be rewind*/
    virtual bool vRewind() = 0;

    template <typename T>
    T readUnit()
    {
        T buffer;
        this->vRead(&buffer, sizeof(T));
        return buffer;
    }
protected:
    GPStream() {}

};
class GPWStream : public GPRefCount
{
public:
    virtual ~GPWStream() {}
    virtual size_t vWrite(const void* buffer, size_t size) = 0;
    virtual bool vFlush() = 0;
    template<typename T>
    bool writeUnit(T v)
    {
        return this->vWrite(&v, sizeof(T));
    }
protected:
    GPWStream() {}
};
#endif
