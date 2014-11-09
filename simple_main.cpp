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
            GPASSERT(3==p->size());
            const PFLOAT* _p = p->get();
            PFLOAT x = _p[0];
            PFLOAT y = _p[1];
            PFLOAT z = _p[2];
            PFLOAT result = -sqrt((x-0.34)*(x-0.34) + (y-0.77)*(y-0.77) + (z-0.56)*(z-0.56));
            return result;
        }
};

int main()
{
    GPPtr<IGPOptimizor::IComputer> comput = new double_computer;
    GPPtr<IGPOptimizor> opt = GPOptimizorFactory::create(GPOptimizorFactory::PSO_SEARCH);
    GPPtr<GPParameter> p = opt->vFindBest(3, comput);
    for (int i=0; i<3;++i)
    {
        cout << p->get(i) << ",";
    }
    cout << endl;
    return 1;
}
