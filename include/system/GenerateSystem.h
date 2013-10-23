#ifndef GENERATE_SYSTEM_H
#define GENERATE_SYSTEM_H

#include "computeSystem.h"
#include "core/GeneticProgram.h"
#include <assert.h>

class GenerateSystem
{
    public:
        GenerateSystem():mComputeSystem(NULL){}
        GenerateSystem(computeSystem* sys ):mComputeSystem(sys){}
        virtual ~GenerateSystem(){}
        std::vector<int> searchOneSequence();
        std::vector<int> searchOneWithOutput(int out);
        virtual std::vector<int> getRandSequence();
        virtual std::vector<int> getRandSequenceWithOutput(int outputFunctionId);
        inline void setComputeSystem(computeSystem* comsys){mComputeSystem = comsys;}
        inline const std::vector<computeFunction>& getMapFunction()
        {
            assert(NULL!=mComputeSystem);
            return mComputeSystem->getMapFunction();
        }
    protected:
        computeSystem* mComputeSystem;

};
#endif
