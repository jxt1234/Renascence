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
#include "GPBasicAdaptor.h"
#include "protobuf2json.h"
#include <fstream>


IGPFrontEnd* GPCompilerCreator::createFront()
{
    return new GPFrontEnd;
}
IGPMidEnd* GPCompilerCreator::createMidEnd()
{
    return new GPBasicMidEnd;
}

IGPBackEnd* GPCompilerCreator::createBackEnd(const GPFunctionDataBase* base)
{
    return new GPBackEnd(base);
}

IGPAdaptor* GPCompilerCreator::createAdaptor(const GPFunctionDataBase* base)
{
    return new GPBasicAdaptor(base);
}

static void _saveProto(void* m, std::ofstream& output)
{
    auto size = protobuf_c_message_get_packed_size((const ProtobufCMessage *)m);
    uint8_t* buffer = (uint8_t*)::malloc(size*sizeof(uint8_t));
    protobuf_c_message_pack((const ProtobufCMessage *)m, buffer);
    char* jsonString = NULL;
    protobuf2json_string((ProtobufCMessage *)m, 0, &jsonString, NULL, 0);
    output<<jsonString;
    ::free(buffer);
    ::free(jsonString);
}

std::function<IGPFunction*(const char*, char**)> GPCompilerCreator::createBasicCompiler(const GPFunctionDataBase* base, bool support_adf)
{
    GPPtr<IGPFrontEnd> front = new GPFrontEnd;
    GPPtr<IGPMidEnd> mid = new GPBasicMidEnd;
    GPPtr<IGPBackEnd> back = new GPBackEnd(base);
    if (!support_adf)
    {
        return [front,mid,back](const char* formula, char** error) {
            auto tree = front->vCreate(formula, error);
            GPASSERT(NULL!=tree);//TODO, check compile error
            auto dag = mid->vCreate(tree);
            GPASSERT(NULL!=dag);
            protobuf_c_message_free_unpacked((ProtobufCMessage*)tree, NULL);
            auto f = back->vCreate(dag);
            GPASSERT(NULL!=f);
            protobuf_c_message_free_unpacked((ProtobufCMessage*)dag, NULL);
            return f;
        };
    }
    GPPtr<IGPAdaptor> adaptor = new GPBasicAdaptor(base);
    return [front,mid,back, adaptor](const char* formula, char** error) {
        auto tree = front->vCreate(formula, error);
        GPASSERT(NULL!=tree);//TODO, check compile error
        adaptor->vExpand(tree);
        auto dag = mid->vCreate(tree);
        GPASSERT(NULL!=dag);
        protobuf_c_message_free_unpacked((ProtobufCMessage*)tree, NULL);
        auto f = back->vCreate(dag);
        GPASSERT(NULL!=f);
        protobuf_c_message_free_unpacked((ProtobufCMessage*)dag, NULL);
        return f;
    };
}
