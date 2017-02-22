#include "test/GPTest.h"
#include "compiler/GPCompilerCreator.h"
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include "protobuf2json.h"
#include "proto/GPDAG.pb-c.h"
using namespace std;
static string readFile(const char* name)
{
    ifstream input(name);
    ostringstream output;
    output << input.rdbuf();
    return output.str();
}

class GPMidEndTest:public GPTest
{
    public:
        virtual void run();
        GPMidEndTest(){}
        virtual ~GPMidEndTest(){}
};
void GPMidEndTest::run()
{
    GPPtr<IGPMidEnd> midend = GPCompilerCreator::createMidEnd();
    auto frontJson = readFile("test_resource/pointgroup.json");
    ProtobufCMessage* front = NULL;
    json2protobuf_string(frontJson.c_str(), 0, &gp__point_group__descriptor, &front, NULL, 0);
    GPASSERT(NULL!=front);
    auto dag = midend->vCreate((const GP__PointGroup*)front);
    protobuf_c_message_free_unpacked(front, NULL);
    char* dagjson = NULL;
    protobuf2json_string((ProtobufCMessage*)dag, 0, &dagjson, NULL, 0);
    protobuf_c_message_free_unpacked((ProtobufCMessage*)dag, NULL);
    ofstream output("output/GPMidEndTest.json");
    output << dagjson;
    ::free(dagjson);
}

static GPTestRegister<GPMidEndTest> a("GPMidEndTest");
