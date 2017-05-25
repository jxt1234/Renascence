/******************************************************************
   Copyright 2017, Jiang Xiao-tang

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
#include "GPBasicAdaptor.h"
#include "utils/GPDebug.h"
GPBasicAdaptor::GPBasicAdaptor()
{
    
}

GPBasicAdaptor::~GPBasicAdaptor()
{
    
}

GP__Point* GPBasicAdaptor::_expand(const GP__ADF* adf) const
{
    return NULL;
}

bool GPBasicAdaptor::vExpand(GP__PointGroup* origin) const
{
    GPASSERT(NULL!=origin);
    for (int i=0; i<origin->n_adfs; ++i)
    {
        auto adf = origin->adfs[i];
        GPASSERT(NULL!=adf && NULL == adf->realization);
        adf->realization = _expand(adf);
    }
    return true;
}

int GPBasicAdaptor::vMapStructure(GP__PointGroup* target, const double* paramters, int n_paramters) const
{
    const int BATCH = 3;
    GPASSERT(NULL!=target);
    auto paramterNumber = (int)target->n_adfs*BATCH;
    if (NULL == paramters)
    {
        return paramterNumber;
    }
    GPASSERT(n_paramters == paramterNumber);
    for (int i=0; i<target->n_adfs; ++i)
    {
        auto adf = target->adfs[i];
        GPASSERT(NULL!=adf && NULL!=adf->realization);
        auto p = paramters + BATCH*i;
        auto p1 = p[0];
        auto p2 = p[1];
        auto p3 = p[2];
        if (p1  < 0.3)
        {
            continue;
        }
    }
    return paramterNumber;
}
