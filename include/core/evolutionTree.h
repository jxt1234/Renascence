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
#ifndef CORE_EVOLUTION_TREE_H
#define CORE_EVOLUTION_TREE_H

#include "core/AbstractGP.h"
#include "system/GenerateSystem.h"
#include <fstream>
class evolutionTree:public AbstractGP
{
    public:
        evolutionTree();
        evolutionTree(const evolutionTree& tree);
        void operator=(const evolutionTree& tree);
        double fit_comput();
        double get_fit() const;
        void mutate();
        void print_f(std::ofstream& file){}
        void print(){}//可选
        static void data_input(){}//Load data
        static void destroy(){}//Free data
        static void setGenSystem(GenerateSystem* gen){mGen = gen;}
    protected:
        int _randDetermine();
        static GenerateSystem* mGen;
        static float gLargeVary;
        static float gStatusVary;
};

#endif
