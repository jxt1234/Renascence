/******************************************************************
 Copyright 2015, Jiang Xiao-tang
 
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

#ifndef __GP__GPProducerUtils__H
#define __GP__GPProducerUtils__H
#include "core/GPFunctionDataBase.h"
#include "utils/RefCount.h"
class GPProducerUtils:public RefCount
{
public:
    typedef const GPFunctionDataBase::function* FUNC;
    typedef const IStatusType* TYPE;
    struct func:public RefCount
    {
        FUNC basic;
        std::vector<TYPE> inputs;
        std::vector<TYPE> outputs;
        std::vector<TYPE> childrenInputs;
        std::vector<int> useChildrenInput;//the size must be the same as basic->inputTypes
        std::vector<std::vector<func*>> tables;
    };
    GPProducerUtils(const GPFunctionDataBase* base);
    virtual ~GPProducerUtils();
    inline const std::vector<GPPtr<func>>& get() const {return mFunctions;}
    
    /*TODO: support multi-output*/
    std::vector<func*> getFunctionsForOutput(TYPE t) const;
private:
    std::vector<func*> _getFunctionsForOutput(TYPE t, const std::vector<GPPtr<func>>& lists) const;
    std::vector<GPPtr<func>> mFunctions;
};

#endif /* defined(__GP__GPProducerUtils__) */
