#include "api/evolution.h"
#include "xml/xmlGPLoader.h"

#include <iostream>
using namespace std;

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
