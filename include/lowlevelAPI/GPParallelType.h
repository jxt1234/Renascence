/******************************************************************
 Copyright 2016, Jiang Xiao-tang
 
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
#ifndef USER_GPPARALLELTYPE_H
#define USER_GPPARALLELTYPE_H
#include "GPContents.h"
class IGPAutoDefFunction;
class GPSingleTree;

/*Create By Renascence, Used by low level lib*/
struct GPParallelType
{
    IGPAutoDefFunction* pFunc;
    GPContents* (*pFuncRunProc)(IGPAutoDefFunction* pFunc, GPContents* inputs);
    void (*pFuncFree)(IGPAutoDefFunction*);

    GPSingleTree* pCondition;
    void (*pConditionFree)(GPSingleTree*);
    bool (*pConditionRunProc)(GPSingleTree* tree, double* values, unsigned int valueNumber);
    //If low level lib user thought the GPSingleTree is too slow, can extract formula instead
    std::string sFormula;

    std::vector<std::vector<unsigned int>> mSplitInfo;
    std::vector<std::vector<unsigned int>> mOutputKey;

    void clear()
    {
        pFuncFree(pFunc);
        pConditionFree(pCondition);
    }
};

#endif
