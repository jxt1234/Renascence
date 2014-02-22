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
#ifndef EVOLUTION_EVOLUTIONTREE_H
#define EVOLUTION_EVOLUTIONTREE_H
#include "mutateTree.h"
class IFitComputer
{
    public:
        IFitComputer(){}
        virtual ~IFitComputer(){}
        virtual double fit_comput(AbstractGP* gp) = 0;
};

class evolutionTree
{
    public:
        evolutionTree(mutateTree* self=NULL);
        evolutionTree(const evolutionTree& tree);
        void operator=(const evolutionTree& tree);
        ~evolutionTree();
        double fit_comput();
        double get_fit() const;
        void print(){}//可选
        void mutate();
        void print_f(std::ostream& file){}
        static void data_input(){}//Load data
        static void destroy(){if (mGen) mGen -> clearStatusSet();}//Free data
        static void setGenSystem(GenerateSystem* gen){mGen = gen;}
        static void setFitComputer(IFitComputer* com){mFitComputer = com;}
        //FIXME Dangerous API, just for xmlPrint
        inline mutateTree* get(){return mTree;}
    protected:
        mutateTree* _copy(mutateTree* tree);
        double mFit;
        mutateTree* mTree;
        static GenerateSystem* mGen;
        static IFitComputer* mFitComputer;
};

#endif
