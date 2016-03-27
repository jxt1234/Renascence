/******************************************************************
   Copyright 2013, Jiang Xiao-tang

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
#ifndef CORE_IGPAUTODEFFUNCTION_H
#define CORE_IGPAUTODEFFUNCTION_H
#include "lowlevelAPI/GPContents.h"
#include "utils/GPTreeNode.h"
#include "GPFunctionDataBase.h"
#include "GPFunctionTree.h"
#include <istream>
#include <ostream>
#include "head.h"
#include "optimizor/IGPOptimizor.h"

class IGPAutoDefFunction:public GPRefCount
{
public:
    virtual GPContents* vRun(GPContents* inputs) = 0;
    virtual GPTreeNode* vSave() const = 0;

    /*Set self Parameters without change the structure, Return the number of parameter needed, do nothing if para==null, the number of vMap will not change*/
    virtual int vMap(GPParameter* para) = 0;

    //Basic Function
    IGPAutoDefFunction(){}
    virtual ~IGPAutoDefFunction(){}
    
    void setBasicTree(GPPtr<GPFunctionTree> basic) {mBasic = basic;}
    GPPtr<GPFunctionTree> getBasicTree() const {return mBasic;}
    void setParameters(GPPtr<GPParameter> para) {mPara = para;}
    GPPtr<GPParameter> getParameters() const {return mPara;}
private:
    GPPtr<GPFunctionTree> mBasic;
    GPPtr<GPParameter> mPara;
};

#endif
