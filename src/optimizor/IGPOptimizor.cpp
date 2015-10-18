#include "optimizor/IGPOptimizor.h"
#include "utils/GPDebug.h"
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

void GPParameter::clear(PFLOAT f)
{
    for (int i=0; i<mNum; ++i)
    {
        mContent[i] = f;
    }
}

void GPParameter::makeValid()
{
    for (int i=0; i<mNum; ++i)
    {
        auto f = mContent[i];
        if (f < 0.0)
        {
            f = 0.0;
        }
        if (f>1.0)
        {
            f = 1.0;
        }
        mContent[i] = f;
    }
}
