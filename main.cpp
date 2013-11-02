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
void print(GP_Output& out)
{
    if (out.output.size()>0)
    {
		double* result = (double*)(out.output[0].content);
		cout <<"result = "<<(*result)<<endl;
	}
}

int main()
{
    GP_evolution(10, 100, "func.xml", NULL, "result.xml");
    GP_RunXml("result.xml", NULL, print);
    return 1;
}
