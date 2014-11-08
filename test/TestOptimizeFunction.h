#include <math.h>
#include <optimizor/IGPOptimizor.h>
class TestOptimizeFunction:public IGPOptimizor::IComputer
{
    public:
        virtual PFLOAT run(GPPtr<GPParameter> p)
        {
            GPASSERT(2==p->size());
            const PFLOAT* _p = p->get();
            PFLOAT x = _p[0];
            PFLOAT y = _p[1];
            PFLOAT result = -sqrt((x-0.3)*(x-0.3) + (y-0.334)*(y-0.334));
            return result;
        }
};
