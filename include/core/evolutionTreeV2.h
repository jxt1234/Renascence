#ifndef EVOLUTIONTREE_V2_H
#define EVOLUTIONTREE_V2_H

#include "core/GeneticProgram.h"
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
        void print_f(ofstream& file);
    protected:
        void mutate_struct();
        void mutate_parameter();
        double mFit;
        static GenerateSystem* mGen;
        static float mStructVaryProb;
};

#endif
