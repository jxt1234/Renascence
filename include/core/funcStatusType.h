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
//NAME_alloc Alloc and initialize the status
typedef void*(*statusAllocMethod)();
//NAME_free Denitialize the status and free memory
typedef void(*statusFreeMethod)(void*);
//NAME_vary Randomly vary the status
typedef void(*statusVaryMethod)(void*);
//NAME_map Map (0,1) value to a status
typedef void(*statusMapMethod)(void*, double);
//NAME_copy Copy the status from src to dst, then the mutate of src will not affect dst
typedef void(*statusCopyMethod)(void* src, void* dst);
//NAME_print Print the status to outputstream
typedef void(*statusPrintMethod)(std::ostream&, void*);
//NAME_load Load the status from inputstream
typedef void*(*statusLoadMethod)(std::istream&);

class funcStatusType:public IStatusType
{
    public:
        /*
           The function is assert to be :
           type_alloc,
           type_free,
           type_vary,
           type_map,
           type_copy,
           type_print,
           type_load
         */
        funcStatusType(const std::string& name, IFunctionTable* table);
        virtual ~funcStatusType(){}
        virtual void* Alloc() const;
        virtual void Free(void* contents) const;
        virtual void mutate(void* contents) const;
        virtual void mapValue(void* contents, double value) const;
        virtual void copy(void* src, void* dst) const;
        virtual void print(std::ostream& out, void* contents) const;
        virtual void* load(std::istream& in) const;

        virtual bool vQuery(FUNC f) const;
    private:
        statusAllocMethod allocf;
        statusFreeMethod freef;
        statusVaryMethod mutatef;
        statusMapMethod mapf;
        statusCopyMethod copyf;
        statusPrintMethod printvf;
        statusLoadMethod loadf;
};
#endif
