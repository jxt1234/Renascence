#include <string.h>
#include "optimizor/GPNetSearchOpt.h"
#include "math/carryArray.h"
#include "utils/GPDebug.h"
GPNetSearchOpt::GPNetSearchOpt(PFLOAT inter)
{
    GPASSERT(inter <= 0.5);
    mPrecision = (int)((1.0-inter)/inter) + 2;
    mInter = 1.0/(mPrecision-1);
}
GPNetSearchOpt::~GPNetSearchOpt()
{
}

GPPtr<GPParameter> GPNetSearchOpt::vFindBest(int n, IGPOptimizor::OPTFUNC computer, PFLOAT* target) const
{
    GPASSERT(n>0);
    carryArray array(n, mPrecision);
    GPPtr<GPParameter> result = new GPParameter(n);

    PFLOAT max_fit = computer(result);
    PFLOAT* _result = result->attach();
    GPPtr<GPParameter> p = new GPParameter(n);
    for (;!array.reachEnd();++array)
    {
        const std::vector<int>& cur = array.getCurrent();
        PFLOAT* _p = p->attach();
        for (int i=0; i<n; ++i)
        {
            PFLOAT v = cur[i]*mInter;
            if (v > 1.0) v = 1.0;
            _p[i] = v;
        }
        PFLOAT fit = computer(p);
        if (fit > max_fit)
        {
            max_fit = fit;
            ::memcpy(_result, _p, sizeof(PFLOAT)*n);
        }
    }
    return result;
}
