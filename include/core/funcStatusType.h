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
#ifndef CORE_FUNCSTATUSTYPE_H
#define CORE_FUNCSTATUSTYPE_H
#include "status.h"
#include "user/IFunctionTable.h"
typedef void*(*statusAllocMethod)();
typedef void(*statusVaryMethod)(void*);
typedef void(*statusCopyMethod)(void* src, void* dst);
typedef std::string(*statusPrintMethod)(void*);
typedef void*(*statusLoadMethod)(std::string);

class funcStatusType:public IStatusType
{
    public:
        /*
           The function is assert to be :
           type_alloc,
           type_free,
           type_vary,
           type_copy,
           type_print,
           type_load
         */
        funcStatusType(const std::string& name, IFunctionTable* table);
        virtual ~funcStatusType(){}
        virtual void* salloc() const;
        virtual void sfree(void* contents) const;
        virtual void mutate(void* contents) const;
        virtual void copy(void* src, void* dst) const;
        virtual void print(std::ostream& out, void* contents) const;
        virtual void* load(std::istream& in) const;
    private:
        statusAllocMethod allocf;
        statusVaryMethod freef;
        statusVaryMethod mutatef;
        statusCopyMethod copyf;
        statusPrintMethod printf;
        statusLoadMethod loadf;
};
#endif
