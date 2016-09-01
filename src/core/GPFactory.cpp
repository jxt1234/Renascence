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
#include "core/GPProducer.h"
#include "frontend/GPFunctionFrontEndProducer.h"
#include "backend/GPTreeProducer.h"
#include "backend/GPStreamADFProducer.h"
#include "backend/GPBasicPiecesFunctionCreator.h"
#include "backend/GPDAGPiecesFunctionCreator.h"
#include "backend/GPTreePiecesFunctionCreator.h"
#include "core/GPStreamFactory.h"
#include "xml/xmlReader.h"

GPProducer* GPFactory::createProducer(const GPFunctionDataBase* base, GPFactory::TYPE t)
{
    GPPtr<GPFrontEndProducer> front = new GPFunctionFrontEndProducer(base);
    GPPtr<GPBackEndProducer> back;
    switch(t)
    {
        case TREE:
            back = new GPTreeProducer();
            break;
        case STREAM:
            back = new GPStreamADFProducer();
            break;
        default:
            GPASSERT(0);
            break;
    }
    return new GPProducer(front.get(), back.get(), base);
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

GPPiecesFunctionCreator* GPFactory::createPieceFunctionProducer(const GPProducer* producer, const GPFunctionDataBase* base, const std::map<std::string, std::string>& map_reduce_formula)
{
    return new GPDAGPiecesFunctionCreator(producer, producer->getFront(), map_reduce_formula);
    //return new GPTreePiecesFunctionCreator(base, producer, producer->getFront(), map_reduce_formula);
}

GPPiecesFunctionCreator* GPFactory::createPieceFunctionProducer(const GPProducer* producer, const GPFunctionDataBase* base, GPStream* metafile)
{
    xmlReader r;
    auto root = r.loadStream(metafile);
    GPASSERT(NULL!=root);
    std::map<std::string, std::string> rules;
    for (auto p : root->getChildren())
    {
        rules.insert(std::make_pair(p->name(), p->attr()));
    }
    //return new GPTreePiecesFunctionCreator(base, producer, producer->getFront(), rules);
    return new GPDAGPiecesFunctionCreator(producer, producer->getFront(), rules);
}

GPParallelMachineSet* GPFactory::createParallelSet(const char* metafile, IFunctionTable* t)
{
    GPParallelMachineSet* set = new GPParallelMachineSet;
    xmlReader r;
    GPPtr<GPStreamWrap> s = GPStreamFactory::NewStream(metafile, GPStreamFactory::FILE);
    auto root = r.loadStream(s.get());
    GPASSERT(NULL!=root);
    set->addFunction(root, t);
    return set;
}
GPParallelMachineSet* GPFactory::createParallelSet(GPStream* metafile, IFunctionTable* t)
{
    GPParallelMachineSet* set = new GPParallelMachineSet;
    xmlReader r;
    auto root = r.loadStream(metafile);
    GPASSERT(NULL!=root);
    set->addFunction(root, t);
    return set;
}

