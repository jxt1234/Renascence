#include "test/GPTest.h"
#include "optimizor/GPOptimizorFactory.h"
#include "utils/debug.h"
#include <iostream>
#include "TestOptimizeFunction.h"
using namespace std;

class GPGoldenDividerTest:public GPTest
{
    public:
        virtual void run();
        GPGoldenDividerTest(){}
        virtual ~GPGoldenDividerTest(){}
};
void GPGoldenDividerTest::run()
{
    GPPtr<IGPOptimizor::IComputer> comput = new TestOptimizeFunction;
    GPPtr<IGPOptimizor> opt = GPOptimizorFactory::create(GPOptimizorFactory::GOLDEN_DIVIDE);
    GPPtr<GPParameter> p = opt->vFindBest(2, comput);
    cout << p->get(0) << ", " << p->get(1) << endl;
}

static GPTestRegister<GPGoldenDividerTest> a("GPGoldenDividerTest");