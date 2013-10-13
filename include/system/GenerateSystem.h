#ifndef GENERATE_SYSTEM_H
#define GENERATE_SYSTEM_H

#include "computeSystem.h"
#include "statusSystem.h"

class GenerateSystem
{
    public:
        GenerateSystem():mStatusSystem(NULL), mComputeSystem(NULL){}
        GenerateSystem(computeSystem* sys, statusSystem* status = NULL):mStatusSystem(status), mComputeSystem(sys){}
        virtual ~GenerateSystem(){}
        vector<int> searchOneSequence();
        vector<int> searchOneWithOutput(int out);
        virtual vector<int> getRandSequence();
        virtual vector<int> getRandSequenceWithOutput(int outputFunctionId);
        inline void setComputeSystem(computeSystem* comsys){mComputeSystem = comsys;}
        inline void setStatusSystem(statusSystem* status){mStatusSystem = status;}
        inline mapFunction getMapFunction()
        {
            if (NULL!=mComputeSystem)
            {
                return mComputeSystem->getMapFunction();
            }
            return NULL;
        }
        inline statusMapFunction getStatusMapFunction()
        {
            if (NULL!=mStatusSystem)
            {
                return mStatusSystem->getMapFunction();
            }
            return NULL;
        }
    protected:
        computeSystem* mComputeSystem;
        statusSystem* mStatusSystem;

};
#endif
