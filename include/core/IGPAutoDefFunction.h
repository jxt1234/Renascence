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
#include "user/GPContents.h"
#include "utils/GPTreeNode.h"
#include "GPFunctionDataBase.h"
#include <istream>
#include <ostream>
#include "head.h"
#include "optimizor/IGPOptimizor.h"

class IGPAutoDefFunction:public RefCount
{
public:
    virtual GPContents* vRun(GPContents* inputs) = 0;
    virtual GPTreeNode* vSave() const = 0;
    /*Return a new copy of this Function*/
    virtual IGPAutoDefFunction* vCopy() const= 0;
    
    /*Parameters to determine neighbour structure, return the number of parameter needed, do nothing if para == null, after this API, the number of vMap returned may changed*/
    virtual int vMapStructure(GPParameter* para, bool& changed) = 0;
    
    /*Set self Parameters without change the structure, Return the number of parameter needed, do nothing if para==null, the number of vMap will not change*/
    virtual int vMap(GPParameter* para) = 0;

    //Basic Function
    IGPAutoDefFunction(){}
    virtual ~IGPAutoDefFunction(){}
};

#endif
