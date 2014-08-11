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

class IGPAutoDefFunction:public RefCount
{
    public:
        virtual GP_Output run(const GP_Input& inputs) = 0;
        /*Assume:
          b.save(f); 
          a.load(f); 
          then  a == b
          But a and b must be created by the same GPProducer firstly
         */
        virtual void save(std::ostream& os) {}
        virtual void load(std::istream& is) {}
        /*Return all inputTypes in order*/
        virtual std::vector<const IStatusType*> vGetInputs() const = 0;
        /*Return all outputTypes in order*/
        virtual std::vector<const IStatusType*> vGetOutputs() const = 0;
        //Basic Function
        IGPAutoDefFunction(){}
        virtual ~IGPAutoDefFunction(){}
};

#endif
