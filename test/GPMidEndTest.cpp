#include "test/GPTest.h"
class GPMidEndTest:public GPTest
{
    public:
        virtual void run();
        GPMidEndTest(){}
        virtual ~GPMidEndTest(){}
};
void GPMidEndTest::run()
{
}

static GPTestRegister<GPMidEndTest> a("GPMidEndTest");
