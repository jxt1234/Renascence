#include "test/GPTest.h"
#include "optimizor/GPOptimizorFactory.h"
#include "utils/debug.h"
#include <iostream>
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
    class double_computer:public IGPOptimizor::IComputer
    {
        public:
            virtual PFLOAT run(GPPtr<GPParameter> p)
            {
                GPASSERT(1==p->size());
                const PFLOAT* _p = p->get();
                PFLOAT v = _p[0];
                PFLOAT result = -v*v+1.5*v-0.9;
                return result;
            }
    };
    GPPtr<IGPOptimizor::IComputer> comput = new double_computer;
    GPPtr<IGPOptimizor> opt = GPOptimizorFactory::create(GPOptimizorFactory::GOLDEN_DIVIDE);
    GPPtr<GPParameter> p = opt->vFindBest(1, comput);
    cout << p->get(0) << endl;
}

static GPTestRegister<GPGoldenDividerTest> a("GPGoldenDividerTest");
