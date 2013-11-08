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
#include "api/evolution.h"

#include "core/evolutionTreeV2.h"
#include "core/group.h"
#include "system/xmlGenerateSystem.h"
#include <iostream>
#include <string>
#include <sstream>
#include "xml/xmlGPLoader.h"
using namespace std;

/*TODO Support inputTable*/
/*TODO Invalid Inputs treat*/
void GP_evolution(int number, int times, const char* functionTable, const char* inputTable, const char* outputXml)
{
    srand((unsigned) time(NULL));
    xmlGenerateSystem gen(functionTable, false);
    evolutionTreeV2::setGenSystem(&gen);
    evolution_group<evolutionTreeV2> group(number);
    group.evolution(times);
    evolutionTreeV2* best = group.get_best();
    string result = gen.xmlPrint(best);
    ofstream file;
    file.open(outputXml);
    file<<result;
    file.close();
}

void GP_RunXml(const char* functionXml, const char* input, void(*output)(GP_Output&))
{
    xmlGPLoader load;
    load.loadFile(functionXml);
    GP_Output gp = load.run();
    if (NULL!=output) output(gp);
    GP_Output_clear(gp);
}