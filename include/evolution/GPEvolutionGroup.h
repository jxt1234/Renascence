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
#include <list>
#include <vector>
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
        inline IGPAutoDefFunction* getBest(){return mBest;}
        inline double getBestFit() const {return mBestFit;}
        void loadBest(const GPTreeNode* node);
        void optimizeParameters(IGPAutoDefFunction* g, IGPAutoDefFunction* fit) const;
    private:
        double _fitCompute(IGPAutoDefFunction* g, IGPAutoDefFunction* fit) const;
        void _best(IGPAutoDefFunction* fit);
        void _best(std::function<double(IGPAutoDefFunction*)> f);
        void _expand();
        void _mutate();
        void _clearGroup();
        void _clearBackup();
        void _restoreBackup();

        GPProducer* mSys;
        IGPAutoDefFunction* mBest;
        double mBestFit;
        std::list<IGPAutoDefFunction*> mGroup;
        std::vector<IGPAutoDefFunction*> mBackup;
        std::vector<const IStatusType*> mInput;
        std::vector<const IStatusType*> mOutput;
        int mTime;
        int mSize;
};
#endif
