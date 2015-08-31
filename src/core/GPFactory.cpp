/******************************************************************
   Copyright 2014, Jiang Xiao-tang

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
#include "core/GPFactory.h"
#include "producer/GPTreeProducer.h"
#include "producer/GPGraphicProducer.h"
#include "producer/GPStreamADFProducer.h"
#include "core/GPStreamFactory.h"
#include <fstream>

GPProducer* GPFactory::createProducer(const GPFunctionDataBase* base, GPFactory::TYPE t)
{
    GPProducer* res = NULL;
    switch(t)
    {
        case TREE:
            res = new GPTreeProducer(base);
            break;
        case GRAPHIC:
            res = new GPGraphicProducer(base);
            break;
        case STREAM:
            res = new GPStreamADFProducer(base);
        default:
            break;
    }
    return res;
}
GPFunctionDataBase* GPFactory::createDataBase(const char* metafile, IFunctionTable* t)
{
    GPASSERT(NULL!=metafile);
    GPFunctionDataBase* base = new GPFunctionDataBase;
    GPPtr<GPStreamWrap> s = GPStreamFactory::NewStream(metafile, GPStreamFactory::FILE);
    base->loadXml(s.get(), t);
    return base;
}

GPFunctionDataBase* GPFactory::createDataBase(GPStream* file, IFunctionTable* t)
{
    GPFunctionDataBase* base = new GPFunctionDataBase;
    base->loadXml(file, t);
    return base;
}
