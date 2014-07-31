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
#include "utils/RefCount.h"
#include "user/IFunctionTable.h"

class GPProducer:public statusBasic, public RefCount
{
    public:
        GPProducer(){}
        virtual ~GPProducer(){}
        inline IGPAutoDefFunction* createFunction(int outputTypeId, int inputTypeId = -1)
        {
            std::vector<int> out(1, outputTypeId);
            std::vector<int> inp(1, inputTypeId);
            return this->vCreateFunction(out, inp);
        }
        /*Set up environment*/
        virtual void vAddContent(std::istream& is, IFunctionTable* table) = 0;
        /*Create GP from xml files*/
        virtual IGPAutoDefFunction* vCreateFunctionFromIS(std::istream& is) = 0;
        /*Create a IGPAutoDefFunction which use the inputType to output the same content in outputType, inputRepeat means the content of inputType can be used repeated*/
        /*The function can't be recursive, which can be modified by mutate*/
        virtual IGPAutoDefFunction* vCreateFunction(const std::vector<int>& outputType, const std::vector<int>& inputType, bool inputRepeat = true, bool random = false) = 0;
        virtual std::vector<IGPAutoDefFunction*> vCreateAllFunction(const std::vector<int>& outputType, const std::vector<int>& inputType, bool inputRepeat = true) = 0;
        virtual IGPAutoDefFunction* vCreateFromADF(IGPAutoDefFunction* src) = 0;
        virtual IGPAutoDefFunction* vCreateFunctionFromName(const std::string& name) = 0;
};

#endif
