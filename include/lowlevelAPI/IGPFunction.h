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
#ifndef LOWLEVELAPI_IGPFUNCTION_H
#define LOWLEVELAPI_IGPFUNCTION_H
#include <string>
#include <vector>
#include "IStatusType.h"
#include "GPContents.h"
#include "GPTypes.h"
class IGPFunction : public GPRefCount
{
public:
    virtual GPContents* vRun(GPContents* inputs) = 0;
    virtual ~IGPFunction() {}
    
protected:
    IGPFunction() {}
};
class IKeyFunction : public GPRefCount
{
public:
    virtual unsigned int vRun(unsigned int* values, unsigned int n) = 0;
    virtual ~IKeyFunction() {}
protected:
    IKeyFunction() {}
};
class IGPFunctionContext : public GPRefCount
{
public:
    virtual IGPFunction* vCreateContentFunction(const std::string& formula, const std::string& parameters, const std::vector<const IStatusType*>& inputs) const = 0;
    virtual IKeyFunction* vCreateKeyFunction(const std::string& formula, const std::string& variable) const = 0;
    
    virtual ~IGPFunctionContext() {}
protected:
    IGPFunctionContext() {}
};
#endif
