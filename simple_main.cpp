#include "compiler/GPCompilerCreator.h"
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include "core/GPFactory.h"
#include "core/GPStreamFactory.h"
#include "protobuf2json.h"
#include "proto/GPDAG.pb-c.h"
#include "user/GPAPI.h"

using namespace std;
static string readFile(const char* name)
{
    ifstream input(name);
    ostringstream output;
    output << input.rdbuf();
    return output.str();
}

int main()
{
    GP_Set_Lib_Path("/Users/jiangxiaotang/Documents/Renascence/");
    GP_Set_Stream_Path("/Users/jiangxiaotang/Documents/Renascence/");
    do
    {
        GPPtr<GPFunctionDataBase> base = GPFactory::createDataBase("func.xml", NULL);
        GPPtr<IGPFrontEnd> frontend = GPCompilerCreator::createFront();
        GPPtr<IGPMidEnd> midend = GPCompilerCreator::createMidEnd();
        GPPtr<IGPBackEnd> backend = GPCompilerCreator::createBackEnd(base.get());
        auto frontFormula = readFile("/Users/jiangxiaotang/Documents/Renascence/test_resource/test_backend.txt");
        auto tree = frontend->vCreate(frontFormula.c_str(), NULL);
        auto dag = midend->vCreate(tree);
        protobuf_c_message_free_unpacked((ProtobufCMessage*)tree, NULL);
        GPPtr<IGPFunction> function = backend->vCreate(dag);
        {
            char* dagJson = NULL;
            protobuf2json_string((ProtobufCMessage*)dag, 0, &dagJson, NULL, 0);
            ofstream output("/Users/jiangxiaotang/Documents/Renascence/output/dag_test.json");
            output << dagJson;
        }
        
        protobuf_c_message_free_unpacked((ProtobufCMessage*)dag, NULL);
        
        auto bmp = base->vQueryType("TrBmp");
        GPPtr<GPStream> x0_s = GPStreamFactory::NewStream("input.jpg");
        GPPtr<GPStream> x1_s = GPStreamFactory::NewStream("input_test_simple.jpg");
        GPPtr<GPContents> input = new GPContents;
        input->push(bmp->vLoad(x0_s.get()), bmp);
        input->push(bmp->vLoad(x1_s.get()), bmp);
        GPPtr<GPContents> output = function->vRun(input.get());
        GPASSERT(output->size() == 2);
        GPPtr<GPWStream> y0_s = GPStreamFactory::NewWStream("output/GPBackEndTest_y0.jpg");
        GPPtr<GPWStream> y1_s = GPStreamFactory::NewWStream("output/GPBackEndTest_y1.jpg");
        bmp->vSave(output->get(0), y0_s.get());
        bmp->vSave(output->get(1), y1_s.get());
        
    } while(false);
    return 1;
}
