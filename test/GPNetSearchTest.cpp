#include "test/GPTest.h"
#include "optimizor/GPOptimizorFactory.h"
#include "utils/GPDebug.h"
#include <iostream>
#include <math.h>
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
    auto compute = [](GPPtr<GPParameter> p){
        GPASSERT(2 == p->size());
        const GPFLOAT* _p = p->get();
        GPFLOAT x = _p[0];
        GPFLOAT y = _p[1];
        GPFLOAT result = -sqrt((x-0.3)*(x-0.3) + (y-0.334)*(y-0.334));
        return result;
    };
    GPPtr<IGPOptimizor> opt = GPOptimizorFactory::create(GPOptimizorFactory::NET_SEARCH);
    GPPtr<GPParameter> p = opt->vFindBest(2, compute);
    cout << p->get(0) << ", " << p->get(1) << endl;
}

static GPTestRegister<GPNetSearchTest> a("GPNetSearchTest");
