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
#ifndef EVOLUTION_GPSINGLEOPTIMIZOR_H
#define EVOLUTION_GPSINGLEOPTIMIZOR_H
#include "IGPADFOptimizor.h"
#include "GPADFOptimizorFactory.h"
#include "optimizor/GPOptimizorFactory.h"
class GPSingleOptimizor:public IGPADFOptimizor
{
    public:
        virtual GPPtr<IGPAutoDefFunction> vFindBest(GPPtr<IGPAutoDefFunction> origin) const;
        GPSingleOptimizor(GPPtr<IGPFitComputer> fit, GPOptimizorFactory::TYPE t);
        virtual ~GPSingleOptimizor();
    private:
        GPPtr<IGPFitComputer> mFit;
        GPPtr<IGPOptimizor> mOpt;
};
#endif
