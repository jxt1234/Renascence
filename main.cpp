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

int test_evolution(int num)
{
    srand((unsigned) time(NULL));
    gXmlLoader.loadFile("func.xml");
    xmlGenerateSystem gen("func.xml", false);
    evolutionTreeV2::setGenSystem(&gen);
    evolution_group<evolutionTreeV2> group(10);
    group.evolution(num);
    evolutionTreeV2* best = group.get_best();
    string result = gen.xmlPrint(best);
    ofstream file;
    file.open("result.xml");
    file<<result;
    file.close();
    cout << "fit = "<<best->fit_comput()<<endl;;
    //vector<int> saveFunc(1,5);
    //best->save(gen.getMapFunction(), saveFunc);
    //vector<computeFunction> newFunction = gen.getMapFunction();
    //swapOutput(newFunction);
    ////best->compute(gen.getMapFunction());
    //best->compute(newFunction);
    return 1;
}

#include "xml/xmlGPLoader.h"
int test_run()
{
    xmlGPLoader load;
    load.loadFile("result.xml");
    GP_Output gp = load.run();
    cout <<"GPOUPUT   "<<gp.output.size()<<endl;
    double* result = (double*)gp.output[0].content;
    cout <<"result = "<<(*result)<<endl;
    free(result);
}

int main()
{
    test_evolution(1);
    test_run();
    return 1;
}
