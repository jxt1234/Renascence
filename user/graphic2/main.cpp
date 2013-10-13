#include "core/GeneticProgram.h"
#include "system/computeSystem.h"
#include "system/statusSystem.h"
#include "system/xmlGenerateSystem.h"
#include "utils/debug.h"
#include <time.h>
#include "core/evolutionTreeV2.h"
#include "core/group.h"
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

xmlFunctionLoader gXmlLoader;

string func(int id)
{
    return gXmlLoader.getFuncName(id);
}

string func_stat(int id)
{
    return gXmlLoader.getStatusName(id);
}

computeFunction selfMap(int id)
{
	if (id == 2) id = 7;
	computeFunction result = gAllFunctions[id];
	return result;
}

int main()
{
    srand((unsigned) time(NULL));
    gXmlLoader.loadFile("func.xml");
    xmlGenerateSystem gen("func.xml");
    evolutionTreeV2::setGenSystem(&gen);
    evolution_group<evolutionTreeV2> group(10);
    group.evolution(100);
    evolutionTreeV2* best = group.get_best();
    cout << best->xmlPrint(func);
    vector<int> saveFunc(1,5);
    best->save(gen.getMapFunction(), saveFunc);
    cout << best->fit_comput();
	//best->compute(gen.getMapFunction());
	best->compute(selfMap);
    return 1;
}
