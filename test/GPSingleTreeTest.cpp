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
    "x!=y&&y!=z",
    "x==y||y==z",
};
void GPSingleTreeTest::run()
{
    unsigned int v[] = {5, 8, 1};
    for (int i=0; i<sizeof(gFormulas)/sizeof(const char*); ++i)
    {
        GPPtr<GPSingleTree> tree = GPSingleTree::createFromFormula(gFormulas[i], "x y z");
        FUNC_PRINT(tree->compute(v));
    }
}

static GPTestRegister<GPSingleTreeTest> a("GPSingleTreeTest");
