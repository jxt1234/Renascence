#ifndef CARRY_POINT_H
#define CARRY_POINT_H
#include <vector>
#include <stdlib.h>
using namespace std;
class carryPoint
{
public:
    carryPoint():mDepend(NULL), mGrow(false){}
    virtual ~carryPoint(){}
    virtual bool vGrow() = 0;
    virtual bool vNext() = 0;
    bool grow();
    bool next();
    void clear();
    carryPoint* mDepend;
    vector<carryPoint*> mChild;
    bool mGrow;
};



#endif
