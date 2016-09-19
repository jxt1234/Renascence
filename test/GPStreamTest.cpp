#include "test/GPTest.h"
#include "core/GPStreamFactory.h"
#include "utils/GPDebug.h"
#include <string>
#include <sstream>
class GPStreamTest:public GPTest
{
    public:
        virtual void run();
        GPStreamTest(){}
        virtual ~GPStreamTest(){}
};
void GPStreamTest::run()
{
    std::string content = "You are a fool";
    std::ostringstream os;
    GPPtr<GPWStream> stream = GPStreamFactory::NewWStreamFromStl(os);
    stream->vWrite(content.c_str(), content.size());
    stream->vFlush();
    std::string newContent = os.str();
    FUNC_PRINT_ALL(newContent.c_str(), s);
    FUNC_PRINT_ALL(content.c_str(), s);
}

static GPTestRegister<GPStreamTest> a("GPStreamTest");
