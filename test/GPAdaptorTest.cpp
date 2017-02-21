#include "test/GPTest.h"
class GPAdaptorTest:public GPTest
{
    public:
        virtual void run();
        GPAdaptorTest(){}
        virtual ~GPAdaptorTest(){}
};
void GPAdaptorTest::run()
{
}

static GPTestRegister<GPAdaptorTest> a("GPAdaptorTest");
