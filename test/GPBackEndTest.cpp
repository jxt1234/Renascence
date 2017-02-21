#include "test/GPTest.h"
class GPBackEndTest:public GPTest
{
    public:
        virtual void run();
        GPBackEndTest(){}
        virtual ~GPBackEndTest(){}
};
void GPBackEndTest::run()
{
}

static GPTestRegister<GPBackEndTest> a("GPBackEndTest");
