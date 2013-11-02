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
    xmlGenerateSystem gen(functionTable);
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
