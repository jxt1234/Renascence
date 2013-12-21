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
#ifndef CORE_DATABASE_H
#define CORE_DATABASE_H
/*Abstarct Class, used for GP to query status, function name, and relative lib*/
#include <string>
#include <vector>
#include "function.h"
class IDataBase
{
    public:
        virtual void vQueryStatus(int id, std::string& name, std::string& libName) = 0;
        virtual void vQueryFunction(int id, std::string& name, std::string& libName) = 0;
        virtual void vQueryOutput(int functionId, std::vector<int>& output)=0;

        IDataBase(){}
        virtual ~IDataBase(){}
};

class IFunctionDataBase
{
    public:
        IFunctionDataBase(){}
        virtual ~IFunctionDataBase(){}
        virtual computeFunction vGetCompute(int id) = 0;
};

#endif
