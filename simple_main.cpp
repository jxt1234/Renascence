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

int main()
{
    GPPtr<IGPFrontEnd> frontend = GPCompilerCreator::createFront();
    auto frontFormula = readFile("test_resource/example_program.txt");
    auto dag = frontend->vCreate(frontFormula.c_str(), NULL);
    char* dagjson = NULL;
    protobuf2json_string((ProtobufCMessage*)dag, 0, &dagjson, NULL, 0);
    protobuf_c_message_free_unpacked((ProtobufCMessage*)dag, NULL);
    ofstream output("output/pbtest.json");
    output << dagjson;
    ::free(dagjson);
    
    return 1;
}
