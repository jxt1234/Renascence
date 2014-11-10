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
            {
                int groupsize=20, time=1000;
                if (NULL != parameter)
                {
                    std::istream& is = *parameter;
                    int maxTime;
                    is >> maxTime;
                    time = maxTime/groupsize;
                }
                res = new GPParticleSwarmOpt(10.0, groupsize, time);
            }
            break;
        default:
            break;
    };
    return res;
}
