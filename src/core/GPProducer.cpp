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

#include "core/GPProducer.h"
#include <sstream>
#include "math/GPSingleTree.h"

GPProducer::GPProducer(const GPFunctionDataBase* base, bool supportADF)
{
    GPASSERT(NULL!=base);
    mCreateFunction = GPCompilerCreator::createBasicCompiler(base, supportADF);
}
GPProducer::~GPProducer()
{
}
IGPFunction* GPProducer::vCreateContentFunction(const std::string& formula) const
{
    auto function = mCreateFunction(formula.c_str(), NULL);
    return function;
}

