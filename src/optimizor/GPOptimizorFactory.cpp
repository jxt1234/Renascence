#include "optimizor/GPOptimizorFactory.h"
#include "optimizor/GPGoldenDivideOpt.h"
IGPOptimizor* GPOptimizorFactory::create(TYPE t, std::istream* parameter)
{
    IGPOptimizor* res = NULL;
    switch (t)
    {
        case GOLDEN_DIVIDE:
            {
                PFLOAT inter = 0.01;
                if (NULL != parameter)
                {
                    std::istream& is = *parameter;
                    is >> inter;
                }
                res = new GPGoldenDivideOpt(inter);
            }
        default:
            break;
    };
    return res;
}
