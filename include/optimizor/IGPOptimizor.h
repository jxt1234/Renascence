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
#ifndef OPTIMIZOR_IGPOPTIMIZOR_H
#define OPTIMIZOR_IGPOPTIMIZOR_H
#include "utils/RefCount.h"
#include <stdlib.h>
typedef float PFLOAT;
class GPParameter:public RefCount
{
    public:
        GPParameter(int n, const PFLOAT* c = NULL);
        virtual ~GPParameter();
        inline int size() const {return mNum;}
        inline PFLOAT operator[](int n) const {return mContent[n];}
        inline PFLOAT get(int n) const {return mContent[n];}
        const PFLOAT* get() const {return mContent;}
        inline PFLOAT* attach() {return mContent;}
    private:
        PFLOAT* mContent;
        int mNum;
};
class IGPOptimizor:public RefCount
{
    public:
        class IComputer:public RefCount
        {
            public:
                virtual PFLOAT run(GPPtr<GPParameter> p) = 0;
                IComputer(){}
                virtual ~IComputer(){}
        };
        virtual GPPtr<GPParameter> vFindBest(int n, GPPtr<IComputer> computer) const = 0;
        IGPOptimizor(){}
        virtual ~IGPOptimizor(){}
};
#endif
