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
#include "user/GPContents.h"
#include "utils/RefCount.h"
#include "user/IStatusType.h"
#include "user/IFunctionTable.h"
#include "IGPAutoDefFunction.h"

class GPProducer:public RefCount
{
public:
    GPProducer(const std::string name):mName(name){}
    inline const std::string& name()const {return mName;}
    virtual ~GPProducer(){}
    inline IGPAutoDefFunction* createFunction(const IStatusType* output, const IStatusType* input = NULL) const
    {
        std::vector<const IStatusType*> out(1, output);
        std::vector<const IStatusType*> inp(1, input);
        return this->vCreateFunction(out, inp);
    }
    virtual void vPrint(std::ostream& os) const{}
    /*Create GP from tree structure*/
    virtual IGPAutoDefFunction* vCreateFunctionFromNode(const GPTreeNode* node) const = 0;
    /*Create a IGPAutoDefFunction which use the inputType to output the same content in outputType, inputRepeat means the content of inputType can be used repeated*/
    /*The function can't be recursive, which can be modified by mutate*/
    virtual IGPAutoDefFunction* vCreateFunction(const std::vector<const IStatusType*>& outputType, const std::vector<const IStatusType*>& inputType, bool inputRepeat = true, bool random = false) const = 0;
    virtual std::vector<IGPAutoDefFunction*> vCreateAllFunction(const std::vector<const IStatusType*>& outputType, const std::vector<const IStatusType*>& inputType, bool inputRepeat = true) const = 0;
    /*Construct ADF by a function name, just package the function to be an ADF, the inputFuncs of this function will be ignored*/
    virtual IGPAutoDefFunction* vCreateFunctionFromName(const std::string& name) const = 0;
    /*Construct ADF by formula such as [ f3(f1(x0), f2(x1), f4(f5(x2))) ], +,-,* will be supported later*/
    virtual IGPAutoDefFunction* vCreateFunctionFromFormula(const std::string& formula) const = 0;
private:
    std::string mName;
};

#endif
