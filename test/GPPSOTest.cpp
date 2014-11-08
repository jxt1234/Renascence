#include "test/GPTest.h"
#include "optimizor/GPOptimizorFactory.h"
#include "utils/debug.h"
#include <iostream>
#include "TestOptimizeFunction.h"
using namespace std;
class GPPSOTest:public GPTest
{
    public:
        virtual void run();
        GPPSOTest(){}
        virtual ~GPPSOTest(){}
};
void GPPSOTest::run()
{
    GPPtr<IGPOptimizor::IComputer> comput = new TestOptimizeFunction;
    GPPtr<IGPOptimizor> opt = GPOptimizorFactory::create(GPOptimizorFactory::PSO_SEARCH);
    GPPtr<GPParameter> p = opt->vFindBest(2, comput);
    cout << p->get(0) << ", " << p->get(1) << endl;
}

static GPTestRegister<GPPSOTest> a("GPPSOTest");
