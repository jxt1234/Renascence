#include <iostream>
#include "head.h"
#include "optimizor/IGPOptimizor.h"
#include "optimizor/GPOptimizorFactory.h"
#include <math.h>
using namespace std;

class double_computer:public IGPOptimizor::IComputer
{
    public:
        virtual PFLOAT run(GPPtr<GPParameter> p)
        {
            GPASSERT(2==p->size());
            const PFLOAT* _p = p->get();
            PFLOAT x = _p[0];
            PFLOAT y = _p[1];
            PFLOAT result = -sqrt((x-0.34)*(x-0.34) + (y-0.77)*(y-0.77));
            return result;
        }
};

int main()
{
    GPPtr<IGPOptimizor::IComputer> comput = new double_computer;
    GPPtr<IGPOptimizor> opt = GPOptimizorFactory::create(GPOptimizorFactory::PSO_SEARCH);
    GPPtr<GPParameter> p = opt->vFindBest(2, comput);
    cout << p->get(0) << ", " << p->get(1) << endl;
    return 1;
}
