#include "test/GPTest.h"
#include "compiler/GPCompilerCreator.h"
#include "core/GPFactory.h"
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include "protobuf2json.h"
#include "proto/GPDAG.pb-c.h"
#include "core/GPStreamFactory.h"

using namespace std;

static string readFile(const char* name)
{
    ifstream input(name);
    ostringstream output;
    output << input.rdbuf();
    return output.str();
}

class GPBackEndTest:public GPTest
{
    public:
        virtual void run();
        GPBackEndTest(){}
        virtual ~GPBackEndTest(){}
};
void GPBackEndTest::run()
{
    GPPtr<GPFunctionDataBase> base = GPFactory::createDataBase("func.xml", NULL);
    {
        GPPtr<IGPFrontEnd> frontend = GPCompilerCreator::createFront();
        GPPtr<IGPMidEnd> midend = GPCompilerCreator::createMidEnd();
        GPPtr<IGPBackEnd> backend = GPCompilerCreator::createBackEnd(base.get());
        auto frontFormula = readFile("test_resource/test_backend.txt");
        auto tree = frontend->vCreate(frontFormula.c_str(), NULL);
        auto dag = midend->vCreate(tree);
        protobuf_c_message_free_unpacked((ProtobufCMessage*)tree, NULL);
        GPPtr<IGPFunction> function = backend->vCreate(dag);
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
    }
}

static GPTestRegister<GPBackEndTest> a("GPBackEndTest");
