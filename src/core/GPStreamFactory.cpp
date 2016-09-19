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

#include <stdio.h>
#include <sstream>
#include "core/GPStreamFactory.h"
#include "GPFileStream.h"
#include "GPUserStream.h"
#include "GPStlStream.h"

std::string GPStreamFactory::gPath = "";


const char* GPStreamFactory::getParentPath()
{
    return gPath.c_str();
}

void GPStreamFactory::setParentPath(const char* path)
{
    gPath = path;
}

GPStream* GPStreamFactory::NewStream(const char* meta, MODE m)
{
    GPStream* r = NULL;
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
GPWStream* GPStreamFactory::NewWStream(const char* meta, MODE m)
{
    GPWStream* r = NULL;
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
GPStream* GPStreamFactory::NewStreamFromStl(std::istream& istream)
{
    return new GPStlStream(istream);
}

GPWStream* GPStreamFactory::NewWStreamFromStl(std::ostream& ostream)
{
    return new GPStlWStream(ostream);
}

bool GPStreamFactory::fileExist(const char* path)
{
    std::ostringstream os;
    os <<gPath << path;
    return GPFileStream::exist(os.str().c_str());
}
