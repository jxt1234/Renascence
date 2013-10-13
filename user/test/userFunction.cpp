#include "userFunction.h"

void initFunctions(){}
BasicSet* multi_set_double(vector<BasicSet*> inputs)
{
    //Determine the output size
    if (inputs.empty())
    {
        return NULL;
    }
    int minSize = inputs[0]->size;
    for (int i=1; i< inputs.size(); ++i)
    {
        if (minSize > inputs[i]->size)
        {
            minSize = inputs[i]->size;
        }
    }
    //Alloc output Data
    BasicSet* output = new BasicSet(minSize);
    int computeNumber = output->size / sizeof(double);
    vector<double*> inputData;
    for (int i=0; i< inputs.size(); ++i)
    {
        inputData.push_back((double*)inputs[i]->content);
    }
    double* outputData = (double*)output->content;
    for (int i=0; i < computeNumber; ++i)
    {
        *(outputData +i) = 1;
        for (int j=0; j< inputData.size(); ++j)
        {
            *(outputData + i) *= *(inputData[j] + i);
        }
    }
    //Relese input Data
    for (int i=0; i< inputs.size(); ++i)
    {
        delete inputs[i];
    }
    return output;
}

BasicSet* add_reduction_double(vector<BasicSet*> inputs)
{
    BasicSet* output = new BasicSet(sizeof(double));
    double result = 0;
    for (int i=0; i< inputs.size(); ++i)
    {
        double* inputData = (double*)(inputs[i]->content);
        int size = inputs[i]->size / sizeof(double);
        for (int j=0; j < size; ++j)
        {
            result += *(inputData+j);
        }
    }
    double* outputData = (double*)(output->content);
    *outputData = result;
    //Relese input Data
    for (int i=0; i< inputs.size(); ++i)
    {
        delete inputs[i];
    }
    return output;
}


BasicSet* testGenerate()
{
    BasicSet* output = new BasicSet(100*sizeof(double));
    double* outputData = (double*) output->content;
    for (int i=0; i < output->size/sizeof(double); ++i)
    {
        *(outputData+i) = i;
    }
    return output;
}

