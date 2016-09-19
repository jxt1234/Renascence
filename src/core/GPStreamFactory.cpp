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
#include "utils/GPDebug.h"
#include "core/GPStreamFactory.h"
#include "GPFileStream.h"
#include "GPStlStream.h"

std::string GPStreamFactory::gPath = "";
const GPIStreamCreator* GPStreamFactory::gCreator = NULL;



const char* GPStreamFactory::getParentPath()
{
    return gPath.c_str();
}

void GPStreamFactory::setParentPath(const char* path)
{
    gPath = path;
}

GPStream* GPStreamFactory::NewStream(const char* meta)
{
    std::ostringstream os;
    os <<gPath << meta;
    if (NULL == gCreator)
    {
        //TODO, check if the path is exist
        return new GPFileStream(os.str().c_str());
    }
    return gCreator->vRead(os.str().c_str());
}
GPWStream* GPStreamFactory::NewWStream(const char* meta)
{
    std::ostringstream os;
    os <<gPath << meta;
    if (NULL == gCreator)
    {
        return new GPFileWStream(os.str().c_str());
    }
    return gCreator->vWrite(os.str().c_str());
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

void GPStreamFactory::setStreamCreator(const GPIStreamCreator* creator)
{
    GPASSERT(NULL!=creator);
    gCreator = creator;
}

