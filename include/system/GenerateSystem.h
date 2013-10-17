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
        vector<int> searchOneSequence();
        vector<int> searchOneWithOutput(int out);
        virtual vector<int> getRandSequence();
        virtual vector<int> getRandSequenceWithOutput(int outputFunctionId);
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
