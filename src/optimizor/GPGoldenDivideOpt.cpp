/******************************************************************
   Copyright 2016, Jiang Xiao-tang

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
******************************************************************/
#include "optimizor/GPGoldenDivideOpt.h"
#include "utils/GPDebug.h"
#include <iostream>

static inline GPFLOAT getNextPoint(GPFLOAT sta, GPFLOAT fin)
{
    return 0.618*(fin-sta) + sta;
}

static inline GPFLOAT runOnePass(int k, GPFLOAT v, GPPtr<GPParameter> current, IGPOptimizor::OPTFUNC& computer)
{
    GPASSERT(NULL!=current.get());
    GPASSERT(0<=k && k<current->size());
    GPPtr<GPParameter> p = new GPParameter(current->size(), current->get());
    GPFLOAT* _p = p->attach();
    _p[k] = v;
    return computer(p);
}

GPPtr<GPParameter> GPGoldenDivideOpt::vFindBest(int n, IGPOptimizor::OPTFUNC computer, GPFLOAT* target) const
{
    GPASSERT(n>0);
    GPPtr<GPParameter> result = new GPParameter(n);

    GPFLOAT* result_pars = result->attach();
    for (int i=0; i<n; ++i)
    {
        GPFLOAT fin = 1.0;
        GPFLOAT sta = 0.0;
        GPFLOAT t_fin = getNextPoint(sta,fin);
        GPFLOAT t_sta = getNextPoint(sta,t_fin);
        GPFLOAT fit_sta = runOnePass(i, t_sta, result, computer);
        GPFLOAT fit_fin = runOnePass(i, t_fin, result, computer);
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
