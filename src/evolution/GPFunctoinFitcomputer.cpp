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
#include "evolution/GPFunctionFitcomputer.h"

GPFunctionFitcomputer::GPFunctionFitcomputer(const char* name, const GPProducer* producer, const GP_Input& input):mInput(input)
{
    mFunc = producer->vCreateFunctionFromName(name);
}

GPFunctionFitcomputer::~GPFunctionFitcomputer()
{
}

PFLOAT GPFunctionFitcomputer::compute(IGPAutoDefFunction* f) const
{
    GP_Output out = f->run(mInput);
    GP_Input fixinput;
    GP_Output_collect(fixinput, out);
    GP_Output fixoutput = mFunc->run(fixinput);
    out.clear();
    PFLOAT result = 0.0;
    result = *(fixoutput.get<double>(0));
    fixoutput.clear();
    return result;
}
