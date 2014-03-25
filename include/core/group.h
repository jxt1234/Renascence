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
#ifndef INCLUDE_GP_INCLUDE_CORE_GROUP_H
#define INCLUDE_GP_INCLUDE_CORE_GROUP_H
#include <vector>
#include <iostream>
#include <fstream>
#include "genetic_operator.h"
#define DEFAULT_FIT_EVOLUTION_GROUP "cache/fit.txt"
#define DEFAULT_OUTPUT_EVOLUTION_GROUP "output/result.txt"
#define PART_EVOLU 4
//Evolution group for general.
/*
class individual
{
private:
public:
    individual();
    individual(const individual&);
    void operator=(const individual&);
    double fit_comput();
    inline double get_fit();
    void mutate();
    void print_f(ostream& output);
    void print();//可选
};
*/
template <class individual>
class evolution_group
{
private:
    std::vector<individual*> group;
    individual* best;
    group_select<individual> sel;//The type of group_select operator
    mutation<individual> var;//The type of vary operator
    cross<individual> cro;//The type of crossover operator
public:
    evolution_group(const int scale=20,const int _s=0,const int _v=0,const int _c=0)
    {
        individual *temp;
        for(int i=0;i<scale;++i)
        {
            temp=new individual;
            group.push_back(temp);
        }
        best=new individual;
        best->fit_comput();
    }
    ~evolution_group()
    {
        for(int i=0;i<group.size();++i)
        {
            delete group[i];
        }
        group.clear();
        delete best;
    }
    inline individual* get_best(){return best;}
    bool best_decide()
    {
        double temp;
        int n=0;
        for(int i=0;i<group.size();++i)
        {
            (group[i])->fit_comput();
        }
        temp=(group[0])->get_fit();
        for(int i=1;i<group.size();++i)
        {
            if((group[i])->get_fit()>=temp) temp= (group[i])->get_fit(), n=i;
        }
        if(temp>=best->get_fit()) 
        {
            *best = *(group[n]);
            output();
        }
        return true;
    }
    bool evolution_s()
    {
        best_decide();
        sel.sel(group);
        cro.cro(group);
        var.var(group);
        return true;
    }
    bool evolution(int genr=2000, std::ostream* os = NULL)
    {
        for(int i=0;i<genr;++i)
        {
            evolution_s();
            if (NULL!=os) fit_record(*os);
        }
        return true;
    }
    inline void fit_record(std::ostream& file){file<<best->get_fit()<<std::endl;}
    inline void output(const char* file=DEFAULT_OUTPUT_EVOLUTION_GROUP)
    {
        std::ofstream f;
        f.open(file);
        best->print_f(f);
        f<<std::endl<<best->get_fit();
        f.close();
    }
};
#endif
