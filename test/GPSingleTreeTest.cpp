#include "test/GPTest.h"
#include "math/GPSingleTree.h"

class GPSingleTreeTest:public GPTest
{
    public:
        virtual void run();
        GPSingleTreeTest(){}
        virtual ~GPSingleTreeTest(){}
};

static const char* gFormulas[] = {
    "x*y+z",
    "ln((x*z+y))",
    "cos(sin(x+y)*z)",
    "x!=y&&y!=z",
    "x+z*sin(y/x)+z*cos(y/x)"
};
void GPSingleTreeTest::run()
{
    GPFLOAT v[] = {0.5, 0.8, 1};
    for (int i=0; i<sizeof(gFormulas)/sizeof(const char*); ++i)
    {
        GPPtr<GPSingleTree> tree = GPSingleTree::createFromFormula(gFormulas[i], "x y z");
        FUNC_PRINT_ALL(tree->compute(v), f);
    }
}

static GPTestRegister<GPSingleTreeTest> a("GPSingleTreeTest");
