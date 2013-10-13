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
    void sel(vector<individual*> &group);
    void sel_ES(vector<individual*> &group);
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
    inline void var(vector<individual*> &group);
    inline void var_ES(vector<individual*> &group);
};

#include "generic_operator.tem"
#endif
