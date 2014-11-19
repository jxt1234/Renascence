/******************************************************************
   Copyright 2013, Jiang Xiao-tang

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
#ifndef CORE_IGPAUTODEFFUNCTION_H
#define CORE_IGPAUTODEFFUNCTION_H
#include "user/package.h"
#include "GPFunctionDataBase.h"
#include <istream>
#include <ostream>
#include "head.h"
#include "optimizor/IGPOptimizor.h"

class IGPAutoDefFunction:public RefCount
{
    public:
        virtual GP_Output run(const GP_Input& inputs) = 0;
        virtual void save(std::ostream& os) const{}
        /*Return a new copy of this Function*/
        virtual IGPAutoDefFunction* copy() const= 0;
        /*Return the number of parameter needed, do nothing if para.get()==null*/
        virtual int vMap(GPPtr<GPParameter> para) = 0;
        /*Return all inputTypes in order*/
        virtual std::vector<const IStatusType*> vGetInputs() const = 0;
        /*Return all outputTypes in order*/
        virtual std::vector<const IStatusType*> vGetOutputs() const = 0;
        //Basic Function
        IGPAutoDefFunction(){}
        virtual ~IGPAutoDefFunction(){}
};

#endif
