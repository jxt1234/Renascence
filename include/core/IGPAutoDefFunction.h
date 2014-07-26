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
#include "IDataBase.h"
#include "function.h"
#include "status.h"
#include "utils/RefCount.h"
#include <istream>
#include <ostream>
class IGPUnit:public RefCount
{
    public:
        IGPUnit(){}
        virtual ~IGPUnit(){}
        /*The combined function is implement by this circle: input--compute--output */
        //The input function may delete the content of inp, so the inp must be reconstruct after input 
        virtual void input(const GP_Input& inp, int& cur)=0;
        virtual void compute(IRuntimeDataBase* map, statusBasic* sta)=0;
        virtual GP_Output output() = 0;
};

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
        /*For evolution*/
        virtual void mutate(){}

        /*If the function support variable input/output, return -1*/
        //virtual int inputNumber() const {return -1;}
        //virtual int outputNumber() const {return -1;}
        virtual int inputNumber() const = 0; 
        virtual int outputNumber() const = 0;
        //Basic Function
        IGPAutoDefFunction(){}
        virtual ~IGPAutoDefFunction(){}
};

#endif
