#ifndef GENERATE_SYSTEM_H
#define GENERATE_SYSTEM_H

#include "computeSystem.h"
#include "core/GeneticProgram.h"
#include <assert.h>

#include "core/IDataBase.h"

class GenerateSystem:public IFunctionDataBase
{
    public:
        GenerateSystem():mComputeSystem(NULL){}
        GenerateSystem(computeSystem* sys ):mComputeSystem(sys){}
        virtual computeFunction vGetCompute(int id);
        virtual ~GenerateSystem(){}
        std::vector<int> searchOneSequence();
        std::vector<int> searchOneWithOutput(int out);
        virtual std::vector<int> getRandSequence();
        virtual std::vector<int> getRandSequenceWithOutput(int outputFunctionId);
        inline void setComputeSystem(computeSystem* comsys){mComputeSystem = comsys;}
    protected:
        computeSystem* mComputeSystem;

};
#endif
