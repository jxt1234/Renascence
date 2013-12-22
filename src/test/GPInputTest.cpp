#include "test/GPTest.h"
class GPInputTest:public GPTest
{
    public:
        virtual void run()
        {
        }
        GPInputTest(){}
        virtual ~GPInputTest(){}
};

static GPTestRegister<GPInputTest> a("GPInputTest");
