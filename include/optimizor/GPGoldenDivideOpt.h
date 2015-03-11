/******************************************************************
   Copyright 2014, Jiang Xiao-tang

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
#ifndef OPTIMIZOR_GPGOLDENDIVIDEOPT_H
#define OPTIMIZOR_GPGOLDENDIVIDEOPT_H
#include "IGPOptimizor.h"
class GPGoldenDivideOpt:public IGPOptimizor
{
    public:
        GPGoldenDivideOpt(PFLOAT minInterval=0.01):mInter(minInterval){}
        virtual ~GPGoldenDivideOpt(){}
        virtual GPPtr<GPParameter> vFindBest(int n, IGPOptimizor::OPTFUNC computer, PFLOAT* target) const override;
    private:
        PFLOAT mInter;
};
#endif
