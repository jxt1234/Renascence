#include "test/GPTest.h"
class GPFrontEndTest:public GPTest
{
    public:
        virtual void run();
        GPFrontEndTest(){}
        virtual ~GPFrontEndTest(){}
};
void GPFrontEndTest::run()
{
}

static GPTestRegister<GPFrontEndTest> a("GPFrontEndTest");
