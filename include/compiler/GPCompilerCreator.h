/******************************************************************
   Copyright 2017, Jiang Xiao-tang

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
#ifndef COMPILER_GPCOMPILERCREATOR_H
#define COMPILER_GPCOMPILERCREATOR_H
#include "IGPCompilerInterface.h"
#include "core/GPFunctionDataBase.h"
#include <functional>
class GPCompilerCreator
{
public:
    static IGPFrontEnd* createFront();
    static IGPMidEnd* createMidEnd();
    static IGPBackEnd* createBackEnd(const GPFunctionDataBase* base);
    static IGPAdaptor* createAdaptor(const GPFunctionDataBase* base);
    
    
    static std::function<IGPFunction*(const char*, char**)> createBasicCompiler(const GPFunctionDataBase* base, bool support_adf = false);
};

#endif
