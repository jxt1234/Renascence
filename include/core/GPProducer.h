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
#ifndef CORE_GPPRODUCER_H
#define CORE_GPPRODUCER_H
#include "AbstractGP.h"
#include "status.h"

class GPProducer:public statusBasic, public IADFCreator
{
    public:
        GPProducer(int defaultOutput=0):mDefaultOutput(defaultOutput){}
        virtual ~GPProducer(){}
        /*Create a IGPAutoDefFunction which use the inputType to output the same content in outputType, inputRepeat means the content of inputType can be used repeated*/
        /*The function can't be recursive, which can be modified by mutate(TODO)*/
        virtual IGPAutoDefFunction* vCreateFunction(const std::vector<int>& outputType, const std::vector<int>& inputType, bool inputRepeat = true, bool random = false) = 0;
        virtual std::vector<IGPAutoDefFunction*> vCreateAllFunction(const std::vector<int>& outputType, const std::vector<int>& inputType, bool inputRepeat = true) = 0;
        virtual void vDestroyFunction(IGPAutoDefFunction* f);
        virtual void vPrintFunction(std::ostream& out, IGPAutoDefFunction* f){}
        /*The output is the target function Id*/
        bool initGP(AbstractGP* tree, int output = -1, bool random = true);
        bool initGP(AbstractGP* tree, const std::string& type, bool random = true);
        void mutate(AbstractGP* tree);
        void freeStatus(AbstractGP* tree);
    protected:
        AbstractGP* loadGP(IGPAutoDefFunction* f);
        bool initGP(AbstractGP* tree, const std::vector<int>& queue);
        virtual std::vector<int> searchSequence(int output) = 0;
        virtual std::vector<std::vector<int> > searchAllSequence(int output) = 0;
        virtual std::vector<int> searchRandSequence(int output) = 0;
        virtual std::vector<int> searchType(const std::string& type) = 0;
        int mDefaultOutput;
};

#endif
