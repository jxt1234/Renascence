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
#ifndef USER_FUNCTIONTABLE_H
#define USER_FUNCTIONTABLE_H
#include <string>


class IFunctionTable
{
    public:
        template <typename T>
        T get(const std::string& name){return (T)(this->vGetFunction(name));}
        //TODO make this function const
        virtual void* vGetFunction(const std::string& name)=0;
        IFunctionTable(){}
        virtual ~IFunctionTable(){}
};


#endif
