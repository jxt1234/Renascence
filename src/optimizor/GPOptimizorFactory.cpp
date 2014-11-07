#include "optimizor/GPOptimizorFactory.h"
#include "optimizor/GPGoldenDivideOpt.h"
#include "optimizor/GPNetSearchOpt.h"
IGPOptimizor* GPOptimizorFactory::create(TYPE t, std::istream* parameter)
{
    IGPOptimizor* res = NULL;
    PFLOAT inter = 0.01;
    switch (t)
    {
        case GOLDEN_DIVIDE:
            if (NULL != parameter)
            {
                std::istream& is = *parameter;
                is >> inter;
            }
            res = new GPGoldenDivideOpt(inter);
        case NET_SEARCH:
            if (NULL != parameter)
            {
                std::istream& is = *parameter;
                is >> inter;
            }
            res = new GPNetSearchOpt(inter);
        default:
            break;
    };
    return res;
}
