#include "system/xmlGenerateSystem.h"
#include "utils/debug.h"
#include <time.h>
#include "core/evolutionTreeV2.h"
#include "core/group.h"
#include <iostream>
#include <string>
#include <sstream>
using namespace std;


#include "api/evolution.h"

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
    //GP_evolution(10, 100, "func.xml", NULL, "result.xml");
    test_run();
    return 1;
}
