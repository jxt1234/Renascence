#include "optimizor/GPOptimizorFactory.h"
#include "optimizor/GPGoldenDivideOpt.h"
#include "optimizor/GPNetSearchOpt.h"
#include "optimizor/GPParticleSwarmOpt.h"
#include <map>
#include <vector>
#include <string>
#include <sstream>
using namespace std;
static void translate(const char* describe, std::map<std::string, double>& output)
{
    if (NULL == describe)
    {
        return;
    }
    vector<string> divides;
    string copydes = describe;
    int sta = 0;
    int fin;
    int cur = 0;
    while(cur < copydes.size())
    {
        if (copydes[cur] == ',')
        {
            fin = cur;
            divides.push_back(copydes.substr(sta, fin - sta));
            sta = cur+1;
        }
        cur++;
    }
    divides.push_back(copydes.substr(sta, copydes.size() - sta));
    /*combine*/
    for (auto s : divides)
    {
        size_t pos = s.find("=");
        if (pos!=string::npos)
        {
            /*valid pair*/
            double v;
            istringstream os(s.substr(pos+1, s.size()-pos));
            //FUNC_PRINT_ALL(os.str().c_str(), s);
            os >> v;
            output.insert(make_pair(s.substr(0, pos), v));
        }
    }
}

GPPtr<IGPOptimizor> GPOptimizorFactory::create(TYPE t, const char* describe)
{
    GPPtr<IGPOptimizor> res;
    PFLOAT inter = 0.01;
    map<string, double> parameters;
    translate(describe, parameters);
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
                if (parameters.find("time")!=parameters.end())
                {
                    time = parameters["time"];
                }
                res = new GPParticleSwarmOpt(10.0, groupsize, time);
            }
            break;
        default:
            break;
    };
    return res;
}
