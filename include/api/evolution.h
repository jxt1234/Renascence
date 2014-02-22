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
#ifndef API_EVOLUTION_H
#define API_EVOLUTION_H

#include "core/function.h"

extern "C"{
/*Input: number, times, functionTable, inputTable; Output: outputXml*/
void GP_evolution(int number, int times, const char* functionTable, const char* inputTable, const char* outputXml, bool xmlprint);
/*Input: functionTree's xml, input resource, output function to treat GP_Output*/
/*The GP_Output for the output function will be free in this function*/
void GP_RunXml(const char* functionXml, const char* input, void(*output)(GP_Output&));
}


#endif
