#ifndef CORE_EVOLUTION_TREE_H
#define CORE_EVOLUTION_TREE_H

#include "core/AbstractGP.h"
#include "system/GenerateSystem.h"
#include <fstream>
class evolutionTree:public AbstractGP
{
    public:
        evolutionTree();
        double fit_comput();
        inline double get_fit();
        void mutate();
        void print_f(std::ofstream& file){}
        void print(){}//可选
        static void data_input();//Load data
        static void destroy();//Free data
    protected:
        int _randDetermine();
        static GenerateSystem* mGen;
};

#endif
