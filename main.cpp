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

void swapOutput(vector<computeFunction>& func)
{
    computeFunction temp = func[2];
    func[2] = func[7];
    func[7]=temp;
}

int main()
{
    srand((unsigned) time(NULL));
    gXmlLoader.loadFile("func.xml");
    xmlGenerateSystem gen("func.xml", true);
    evolutionTreeV2::setGenSystem(&gen);
    evolution_group<evolutionTreeV2> group(10);
    group.evolution(100);
    evolutionTreeV2* best = group.get_best();
    cout << best->xmlPrint(func);
    vector<int> saveFunc(1,5);
    best->save(gen.getMapFunction(), saveFunc);
    vector<computeFunction> newFunction = gen.getMapFunction();
    swapOutput(newFunction);
    cout << best->fit_comput();
    //best->compute(gen.getMapFunction());
    best->compute(newFunction);
    return 1;
}
