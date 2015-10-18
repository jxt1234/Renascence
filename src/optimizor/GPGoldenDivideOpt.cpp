#include "optimizor/GPGoldenDivideOpt.h"
#include "utils/GPDebug.h"
#include <iostream>

static inline PFLOAT getNextPoint(PFLOAT sta, PFLOAT fin)
{
    return 0.618*(fin-sta) + sta;
}

static inline PFLOAT runOnePass(int k, PFLOAT v, GPPtr<GPParameter> current, IGPOptimizor::OPTFUNC& computer)
{
    GPASSERT(NULL!=current.get());
    GPASSERT(0<=k && k<current->size());
    GPPtr<GPParameter> p = new GPParameter(current->size(), current->get());
    PFLOAT* _p = p->attach();
    _p[k] = v;
    return computer(p);
}

GPPtr<GPParameter> GPGoldenDivideOpt::vFindBest(int n, IGPOptimizor::OPTFUNC computer, PFLOAT* target) const
{
    GPASSERT(n>0);
    GPPtr<GPParameter> result = new GPParameter(n);

    PFLOAT* result_pars = result->attach();
    for (int i=0; i<n; ++i)
    {
        PFLOAT fin = 1.0;
        PFLOAT sta = 0.0;
        PFLOAT t_fin = getNextPoint(sta,fin);
        PFLOAT t_sta = getNextPoint(sta,t_fin);
        PFLOAT fit_sta = runOnePass(i, t_sta, result, computer);
        PFLOAT fit_fin = runOnePass(i, t_fin, result, computer);
        while(fin-sta>mInter)
        {
            if (fit_sta > fit_fin)
            {
                fin = t_fin;
                t_fin = t_sta;
                t_sta = getNextPoint(sta, t_fin);
                fit_fin = fit_sta;
                fit_sta = runOnePass(i, t_sta, result, computer);
            }
            else
            {
                sta = t_sta;
                t_sta = t_fin;
                t_fin = getNextPoint(sta, fin);
                fit_sta = fit_fin;
                fit_fin = runOnePass(i, t_fin, result, computer);
            }
        }
        result_pars[i] = getNextPoint(t_sta, t_fin);
    }
    return result;
}
