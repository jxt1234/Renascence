#include "test/GPTest.h"
#include "compiler/GPCompilerCreator.h"
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include "protobuf2json.h"
#include "proto/GPDAG.pb-c.h"
#include "core/GPFactory.h"
#include <memory>


using namespace std;
static string readFile(const char* name)
{
    ifstream input(name);
    ostringstream output;
    output << input.rdbuf();
    return output.str();
}
class GPAdaptorTest:public GPTest
{
    public:
        virtual void run();
        GPAdaptorTest(){}
        virtual ~GPAdaptorTest(){}
};
void GPAdaptorTest::run()
{
    GPPtr<GPFunctionDataBase> base = GPFactory::createDataBase("func.xml", NULL);
    {
        GPPtr<IGPFrontEnd> frontend = GPCompilerCreator::createFront();
        auto frontFormula = readFile("test_resource/example_program.txt");
        auto dag = frontend->vCreate(frontFormula.c_str(), NULL);
        GPPtr<IGPAdaptor> adaptor = GPCompilerCreator::createAdaptor(base.get());
        adaptor->vExpand(dag);
        char* dagjson = NULL;
        protobuf2json_string((ProtobufCMessage*)dag, 0, &dagjson, NULL, 0);
        protobuf_c_message_free_unpacked((ProtobufCMessage*)dag, NULL);
        ofstream output("output/GPAdaptorTest.json");
        output << dagjson;
        ::free(dagjson);
    }
}

static GPTestRegister<GPAdaptorTest> a("GPAdaptorTest");
