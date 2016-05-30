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
#ifndef CORE_GPSTREAMFACTORY_H
#define CORE_GPSTREAMFACTORY_H
#include "lowlevelAPI/GPStream.h"
#include "lowlevelAPI/GPRefCount.h"
#include <string>
#include <istream>
#include <ostream>
class GPStreamWrap:public GPStream, public GPRefCount
{
public:
    GPStreamWrap();
    virtual ~GPStreamWrap();
};
class GPWStreamWrap:public GPWStream, public GPRefCount
{
public:
    GPWStreamWrap();
    virtual ~GPWStreamWrap();
};

class GPStreamFactory
{
public:
    typedef enum{
        FILE,
        USER
    } MODE;
    static GPStreamWrap* NewStream(const char* meta, MODE m=FILE);
    static GPWStreamWrap* NewWStream(const char* meta, MODE m=FILE);
    
    static GPStreamWrap* NewStreamFromStl(std::istream& istream);
    static GPWStreamWrap* NewWStreamFromStl(std::ostream& ostream);
    static void setParentPath(const char* path);
private:
    static std::string gPath;
};

#endif
