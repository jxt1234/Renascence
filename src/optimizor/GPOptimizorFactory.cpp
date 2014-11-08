#include "optimizor/GPOptimizorFactory.h"
#include "optimizor/GPGoldenDivideOpt.h"
#include "optimizor/GPNetSearchOpt.h"
#include "optimizor/GPParticleSwarmOpt.h"

GPPtr<IGPOptimizor> GPOptimizorFactory::create(TYPE t, std::istream* parameter)
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
            break;
        case NET_SEARCH:
            if (NULL != parameter)
            {
                std::istream& is = *parameter;
                is >> inter;
            }
            res = new GPNetSearchOpt(inter);
            break;
        case PSO_SEARCH:
            res = new GPParticleSwarmOpt(10.0, 2.0, 2.0, 50);
            break;
        default:
            break;
    };
    return res;
}
