#include "test/GPTest.h"
#include "optimizor/GPOptimizorFactory.h"
#include "utils/GPDebug.h"
#include <iostream>
#include <math.h>
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
    auto compute = [](GPPtr<GPParameter> p){
        GPASSERT(2 == p->size());
        const PFLOAT* _p = p->get();
        PFLOAT x = _p[0];
        PFLOAT y = _p[1];
        PFLOAT result = -sqrt((x-0.3)*(x-0.3) + (y-0.334)*(y-0.334));
        return result;
    };
    GPPtr<IGPOptimizor> opt = GPOptimizorFactory::create(GPOptimizorFactory::GOLDEN_DIVIDE);
    GPPtr<GPParameter> p = opt->vFindBest(2, compute);
    cout << p->get(0) << ", " << p->get(1) << endl;
}

static GPTestRegister<GPGoldenDividerTest> a("GPGoldenDividerTest");
