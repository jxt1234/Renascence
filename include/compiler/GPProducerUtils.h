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
#include "lowlevelAPI/GPRefCount.h"
class GPProducerUtils:public GPRefCount
{
public:
    typedef const GPFunction* FUNC;
    typedef const IStatusType* TYPE;
    struct func:public GPRefCount
    {
        FUNC basic;
        std::vector<TYPE> inputs;
        std::vector<TYPE> outputs;
        std::vector<TYPE> childrenInputs;
        std::vector<int> useChildrenInput;//the size must be the same as basic->inputTypes
        std::vector<std::vector<const func*>> tables;
        static func create(FUNC f, bool clean = false);
        static void invalidate(func& f);
        func() = default;
        func(const func& f) = default;
        virtual ~func() = default;
    };
    GPProducerUtils(const GPFunctionDataBase* base);
    virtual ~GPProducerUtils();
    inline const std::vector<GPPtr<func>>& get() const {return mFunctions;}
    inline const GPFunctionDataBase* getOriginBase() const {return mBase;}
    
    /*TODO: support multi-output*/
    std::vector<const func*> getFunctionsForOutput(TYPE t) const;
private:
    void _invalidateTable();
    std::vector<const func*> _getFunctionsForOutput(TYPE t, const std::vector<GPPtr<func>>& lists) const;
    std::vector<GPPtr<func>> mFunctions;
    const GPFunctionDataBase* mBase;
};

#endif /* defined(__GP__GPProducerUtils__) */
