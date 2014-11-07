#include <iostream>
#include "head.h"
#include "optimizor/IGPOptimizor.h"
#include "optimizor/GPGoldenDivideOpt.h"
#include "optimizor/GPNetSearchOpt.h"
using namespace std;

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

int main()
{
    GPPtr<IGPOptimizor::IComputer> comput = new double_computer;
    GPPtr<IGPOptimizor> opt = new GPNetSearchOpt(0.01);
    GPPtr<GPParameter> p = opt->vFindBest(1, comput);
    cout << p->get(0) << endl;
    return 1;
}
