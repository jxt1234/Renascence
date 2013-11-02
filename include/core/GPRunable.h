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
#ifndef CORE_GPRUNABLE_H
#define CORE_GPRUNABLE_H
#include "core/GeneticProgram.h"


class GPRunable:public GeneticProgram
{
    public:
        static void setDataBase(IFunctionDataBase* data){mData = data;}
        /*Compute the result from leaf to root, the inputs is set to savePoint*/
        /*f(a, b) =  f(g(a), (h(b)))*/
        GP_Output inputCompute(std::vector<void*> inputs, std::vector<void*> constValue);
        /*Compute the result from root to leaf, the outpus is put to savePoint, which will be collected and output*/
        /*f(a, b) = g(f(a,b).x), h(f(a,b),y)*/
        GP_Output outputCompute(std::vector<void*> inputs, std::vector<void*> constValue);

        /*Search All SavePoint and their parent*/
    protected:
        /*The points is used as inputs or outputs, they are Generated when begin to compute*/
        /*Each savePoint can has only one input*/
        void refreshSavePoint();
        void createSavePoints(GeneticPoint* point);
        void assignInputs(const std::vector<void*>& inputs);
        std::vector<GeneticPoint*> mSavePoints;
        /*Runtime DataBase*/
        static IFunctionDataBase* mData;
};


#endif
