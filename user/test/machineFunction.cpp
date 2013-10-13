#include "userFunction.h"
BasicSet* multi_set_double(vector<BasicSet*> inputs);
BasicSet* testGenerate();
BasicSet* add_reduction_double(vector<BasicSet*> inputs);
void* func0(vector<void*> inputs, vector<void*> constValue)
{
    vector<BasicSet*> inputsT;
    for (int i=0; i< inputs.size(); ++i)
    {
        inputsT.push_back((BasicSet*)inputs[i]);
    }
    BasicSet* output =    multi_set_double(inputsT);
    return (void*)output;
}
void* func1(vector<void*> inputs, vector<void*> constValue)
{
    vector<BasicSet*> inputsT;
    for (int i=0; i< inputs.size(); ++i)
    {
        inputsT.push_back((BasicSet*)inputs[i]);
    }
    BasicSet* output =    add_reduction_double(inputsT);
    return (void*)output;
}
void* func2(vector<void*> inputs, vector<void*> constValue)
{
    BasicSet* output =    testGenerate();
    return (void*)output;
}
computeFunction gComputeFunctions[] = {func0, func1, func2};

//必须提供的变量与函数定义
computeFunction* gAllFunctions = gComputeFunctions;
int gAllFunctionsNumber = sizeof(gComputeFunctions)/sizeof(computeFunction);

computeFunction basicMap(int functionId)
{
    return gComputeFunctions[functionId % (sizeof(gComputeFunctions)/sizeof(computeFunction))];
}
