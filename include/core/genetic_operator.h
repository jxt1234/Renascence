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
#ifndef GENETIC_OPERTOR_H
#define GENETIC_OPERTOR_H
#include <vector>
using namespace std;
template <class individual>
class group_select
{
private:
    int type;
    int part;
    vector<individual*> optimal;
public:
    group_select(int _t=0,int _p=4):type(_t),part(_p){}
    virtual ~group_select()
    {
        for(int i=0;i<optimal.size();++i) delete (optimal[i]);
        optimal.clear();
    }
    void sel(vector<individual*> &group)
    {
        switch(type)
        {
            case 0:
                sel_ES(group);
                break;
            default:
                break;
        }
    }
    void sel_ES(vector<individual*> &group)
    {
        int n=group.size()/part;
        double temp;
        int tem_n;
        individual* tem_i;
        if(optimal.size()<part)
        {
            for(int i=0;i<part;++i)
            {
                tem_i=new individual;
                temp = (group[i*n])->get_fit();
                tem_n=i*n;
                for(int j=1;j<n;++j)
                {
                    if((group[j+i*n])->get_fit() >= temp)
                    {
                        temp = (group[j+i*n])->get_fit();
                        tem_n = j+i*n;
                    }
                }
                *tem_i = *(group[tem_n]);
                optimal.push_back(tem_i);
            }
        }
        else
        {
            for(int i=0;i<part;++i)
            {
                temp = (group[i*n])->get_fit();
                tem_n=i*n;
                for(int j=1;j<n;++j)
                {
                    if((group[j+i*n])->get_fit() >= temp)
                    {
                        temp = (group[j+i*n])->get_fit();
                        tem_n = j+i*n;
                    }
                }
                if( (group[tem_n])->get_fit() >=(optimal[i])->get_fit()) *(optimal[i])=*(group[tem_n]);
            }
        }
    }
};


template <class individual>
class cross
{
private:
    int type;
public:
    cross(int _t=0):type(_t){}
    virtual ~cross(){}
    inline void cro(vector<individual*> &group){}
    inline void cro_ES(vector<individual*> &group){}
};

template <class individual>
class mutation
{
private:
    int type;
public:
    mutation(int _t=0):type(_t){}
    virtual ~mutation(){}
    inline void var(vector<individual*> &group)
    {
        switch(type)
        {
            case 0:
                var_ES(group);
                break;
            default:
                break;
        }
    }
    inline void var_ES(vector<individual*> &group)
    {
        for(int i=0;i<group.size();++i)
        {
            (group[i])->mutate();
            //(group[i])->print();
        }
    }
};

#endif
