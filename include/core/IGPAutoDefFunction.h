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
class IGPUnit
{
    public:
        IGPUnit(){}
        virtual ~IGPUnit(){}
        /*The combined function is implement by this circle: input--compute--output */
        //The input function may delete the content of inp, so the inp must be reconstruct after input 
        virtual void input(GP_Input& inp, int& cur)=0;
        virtual void compute(IRuntimeDataBase* map, statusBasic* sta)=0;
        virtual GP_Output output() = 0;
};
class IGPAutoDefFunction
{
    public:
        inline GP_Output run(GP_Input inputs)
        {
            int cur = 0;
            mBase->input(inputs, cur);
            mBase->compute(mFuncDataBase, mStausData);
            return mBase->output();
        }
        //Basic Function
        IGPAutoDefFunction(IRuntimeDataBase* runTime, statusBasic* status, IGPUnit* base):mFuncDataBase(runTime), mStausData(status), mBase(base){}
        virtual ~IGPAutoDefFunction(){}
    private:
        IRuntimeDataBase* mFuncDataBase;
        statusBasic* mStausData;
        IGPUnit* mBase;
};
#endif
