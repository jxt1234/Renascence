#include "optimizor/IGPOptimizor.h"
#include "utils/debug.h"
#include <string.h>
GPParameter::GPParameter(int n, const PFLOAT* c)
{
    GPASSERT(n>0);
    mNum = n;
    mContent = new PFLOAT[n];
    if (NULL!=c)
    {
        for (int i=0; i<n; ++i)
        {
            PFLOAT _c = c[i];
            GPASSERT(_c>=0.0 && _c<=1.0);
        }
        ::memcpy(mContent, c, sizeof(PFLOAT)*n);
    }
    else
    {
        ::memset(mContent, 0, sizeof(PFLOAT)*n);
    }
}

GPParameter::~GPParameter()
{
    delete [] mContent;
}
