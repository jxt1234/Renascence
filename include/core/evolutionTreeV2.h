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
#ifndef EVOLUTIONTREE_V2_H
#define EVOLUTIONTREE_V2_H

#include "GeneticProgram.h"
#include "system/GenerateSystem.h"

class evolutionTreeV2:public GeneticProgram
{
    public:
        //For user
        static void setGenSystem(GenerateSystem* sys);
        static void setStructVariProb(float rate);
        //For evolution group API
        static void data_input(){}//Load data
        static void destroy(){}//Free data
        evolutionTreeV2();
        evolutionTreeV2(const evolutionTreeV2& src);
        void operator=(const evolutionTreeV2& src);
        void print(){}
        virtual ~evolutionTreeV2(){}
        double fit_comput();
        inline double get_fit(){return mFit;}
        virtual void mutate();
        void print_f(std::ofstream& file);
    protected:
        void mutate_struct();
        void mutate_parameter();
        double mFit;
        static GenerateSystem* mGen;
        static float mStructVaryProb;
};

#endif
