#include "test/GPTest.h"
#include "optimizor/GPOptimizorFactory.h"
#include "utils/debug.h"
#include <iostream>
#include <math.h>
#include "TestOptimizeFunction.h"
using namespace std;
class GPNetSearchTest:public GPTest
{
    public:
        virtual void run();
        GPNetSearchTest(){}
        virtual ~GPNetSearchTest(){}
};
void GPNetSearchTest::run()
{
    GPPtr<IGPOptimizor::IComputer> comput = new TestOptimizeFunction;
    GPPtr<IGPOptimizor> opt = GPOptimizorFactory::create(GPOptimizorFactory::NET_SEARCH);
    GPPtr<GPParameter> p = opt->vFindBest(2, comput);
    cout << p->get(0) << ", " << p->get(1) << endl;
}

static GPTestRegister<GPNetSearchTest> a("GPNetSearchTest");
