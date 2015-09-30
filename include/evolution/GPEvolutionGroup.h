/******************************************************************
 Copyright 2014, Jiang Xiao-tang
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 ******************************************************************/
#ifndef EVOLUTION_XMLEVOLUTIONGROUP_H
#define EVOLUTION_XMLEVOLUTIONGROUP_H
#include "core/GPProducer.h"
#include "core/IGPAutoDefFunction.h"
#include <vector>
#include <map>
#include <istream>
#include <functional>

/*ES strategy*/
class GPEvolutionGroup:public RefCount
{
public:
    GPEvolutionGroup(GPProducer* sys, int time = 10, int size = 10);
    virtual ~GPEvolutionGroup();
    virtual void vSetInput(const std::vector<const IStatusType*>& input){mInput = input;}
    virtual void vSetOutput(const std::vector<const IStatusType*>& output){mOutput = output;}
    /*The fit function is assume to output only one double value*/
    virtual void vEvolutionFunc(std::function<double(IGPAutoDefFunction*)> fit_func);
    inline GPPtr<IGPAutoDefFunction> getBest(){return mBest;}
    inline double getBestFit() const {return mBestFit;}
    void loadBest(const GPTreeNode* node);
private:
    struct func_para:public RefCount
    {
        GPPtr<GPParameter> pStructure;
        GPPtr<GPParameter> pParamter;
        void init(IGPAutoDefFunction* basic);
        void mutate();
        void invalidate(IGPAutoDefFunction* basic);
    };
    void _best(std::function<double(IGPAutoDefFunction*)>& fitf);
    void _expand();
    void _mutate();
    
    GPProducer* mSys;
    GPPtr<IGPAutoDefFunction> mBest;
    double mBestFit;
    std::vector<GPPtr<IGPAutoDefFunction>> mGroup;
    GPPtr<func_para> mPara;
    
    std::vector<const IStatusType*> mInput;
    std::vector<const IStatusType*> mOutput;
    int mTime;
    int mSize;
};
#endif
