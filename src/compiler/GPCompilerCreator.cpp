/******************************************************************
   Copyright 2017, Jiang Xiao-tang

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
#include "compiler/GPCompilerCreator.h"
#include "GPFrontEnd.h"
#include "GPBackEnd.h"
#include "GPBasicMidEnd.h"


IGPFrontEnd* GPCompilerCreator::createFront()
{
    return new GPFrontEnd;
}
IGPAdaptor* GPCompilerCreator::createAdaptor()
{
    return NULL;
}

IGPMidEnd* GPCompilerCreator::createMidEnd()
{
    return new GPBasicMidEnd;
}

IGPBackEnd* GPCompilerCreator::createBackEnd(const GPFunctionDataBase* base)
{
    return new GPBackEnd(base);
}

std::function<IGPFunction*(const char*, char**)> GPCompilerCreator::createBasicCompiler(const GPFunctionDataBase* base)
{
    GPPtr<IGPFrontEnd> front = new GPFrontEnd;
    GPPtr<IGPMidEnd> mid = new GPBasicMidEnd;
    GPPtr<IGPBackEnd> back = new GPBackEnd(base);
    
    return [front,mid,back](const char* formula, char** error) {
        auto tree = front->vCreate(formula, error);
        GPASSERT(NULL!=tree);//TODO
        auto dag = mid->vCreate(tree);
        GPASSERT(NULL!=dag);
        protobuf_c_message_free_unpacked((ProtobufCMessage*)tree, NULL);
        auto f = back->vCreate(dag);
        GPASSERT(NULL!=f);
        protobuf_c_message_free_unpacked((ProtobufCMessage*)dag, NULL);
        return f;
    };
}
