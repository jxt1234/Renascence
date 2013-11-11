#ifndef CORE_ABSTRACTGP_H
#define CORE_ABSTRACTGP_H
#include <stdlib.h>
#include "algorithm/AbstractPoint.h"
#include "core/function.h"
#include "core/IDataBase.h"
#include "core/IGPRunner.h"
#include "utils/GP_Clock.h"
#include "core/status.h"
class AbstractGPPoint:public AbstractPoint
{
    public:
        AbstractGPPoint():mFunc(-1), mStatus(-1){}
        virtual ~AbstractGPPoint(){}
    protected:
        int mFunc;
        int mStatus;
};

class AbstractGP:public AbstractGPPoint, IGPRunner
{
    public:
        //Basic Functions
        AbstractGP():mSave(NULL){}
        AbstractGP(const AbstractGP& gp);
        void operator=(const AbstractGP& gp);
        virtual ~AbstractGP();
        //Basic API
        virtual void compute(IFunctionDataBase* map);
        virtual void input(GP_Input& input);
        virtual GP_Output output();
        /*Old API to cover GeneticProgram*/
        void reset();//Clear all mSave
        //If the functionIds is empty, it means to save all points
        void save(IFunctionDataBase* map, const std::vector<int>& functionIds);
        //The cur means from which element to use, it's assumed that the numbers is valid and the status has been alloc
        //FIXME: dangerous api
        void replacePoint(const std::vector<int> &numbers, int& cur);
        //Output a xml string, which can be write into file directly
        std::string xmlPrint(IDataBase* data);
    protected:
        GP_Output up_compute(IFunctionDataBase* map);
        void _reset();
        class AbstractGPCopy:public AbstractPoint::IPointCopy
        {
            public:
                virtual AbstractPoint* copy(AbstractPoint* src)
                {
                    AbstractGP* result = new AbstractGP;
                    AbstractGP* s = (AbstractGP*)src;
                    result->mFunc = s->mFunc;
                    result->mStatus = status_CopyAllocSet(s->mStatus);
                    return result;
                }
        };
        GP_Output* mSave;
};


#endif
