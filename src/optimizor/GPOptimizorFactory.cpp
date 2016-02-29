#include "optimizor/GPOptimizorFactory.h"
#include "optimizor/GPGoldenDivideOpt.h"
#include "optimizor/GPNetSearchOpt.h"
#include "optimizor/GPParticleSwarmOpt.h"
#include <map>
#include <vector>
#include <string>
#include <sstream>
using namespace std;


GPPtr<IGPOptimizor> GPOptimizorFactory::create(TYPE t, int maxTimes)
{
    GPPtr<IGPOptimizor> res;
    GPFLOAT inter = 0.01;
    switch (t)
    {
        case GOLDEN_DIVIDE:
            res = new GPGoldenDivideOpt(inter);
            break;
        case NET_SEARCH:
            res = new GPNetSearchOpt(inter);
            break;
        case PSO_SEARCH:
            {
                int groupsize=20, time=1000;
                if (maxTimes > 20)
                {
                    time = maxTimes/20;
                }
                res = new GPParticleSwarmOpt(10.0, groupsize, time);
            }
            break;
        default:
            break;
    };
    return res;
}
