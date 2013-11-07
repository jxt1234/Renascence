#ifndef CORE_ABSTRACTGP_H
#define CORE_ABSTRACTGP_H
#include <stdlib.h>
#include "algorithm/AbstractPoint.h"
#include "core/function.h"

class AbstractGPPoint:public AbstractPoint
{
    public:
        AbstractGPPoint():mFunc(-1), mStatus(-1){}
        virtual ~AbstractGPPoint(){}
    protected:
        int mFunc;
        int mStatus;
};

class AbstractGP:public AbstractGPPoint
{
    public:
        AbstractGP():mSave(NULL){}
        AbstractGP(const AbstractGP& gp);
        void operator=(const AbstractGP& gp);
        virtual ~AbstractGP();
    protected:
        GP_Output* mSave;
};


#endif
