#include "test/GPTest.h"
#include "math/GPSingleTree.h"

class GPSingleTreeTest:public GPTest
{
    public:
        virtual void run();
        GPSingleTreeTest(){}
        virtual ~GPSingleTreeTest(){}
};
void GPSingleTreeTest::run()
{
    GPPtr<GPSingleTree> tree = GPSingleTree::createFromFormula("x*y+z", "x y z");
    GPFLOAT v[3];
    v[0] = 0.5;
    v[1] = 0.8;
    v[2] = 1;
    FUNC_PRINT_ALL(tree->compute(v), f);
}

static GPTestRegister<GPSingleTreeTest> a("GPSingleTreeTest");
