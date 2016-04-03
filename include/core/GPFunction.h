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
#ifndef CORE_GPFUNCTION_H
#define CORE_GPFUNCTION_H
#include <string>
#include <vector>
#include "lowlevelAPI/IStatusType.h"
struct GPFunction
{
public:
    //For print
    std::string name;
    std::string shortname;
    //For compute
    computeFunction basic;
    std::vector<const IStatusType*> inputType;
    std::vector<bool> inputNeedComplete;
    std::vector<const IStatusType*> outputType;
    std::vector<const IStatusType*> statusType;
};
#endif
