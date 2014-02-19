#include "test/GPTest.h"
#include "api/evolution.h"

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
class GPRunTest:public GPTest
{
    public:
        virtual void run()
        {
            GP_RunXml("result.xml", NULL, print);
        }
        GPRunTest(){}
        virtual ~GPRunTest(){}
};

static GPTestRegister<GPRunTest> a("RunXmlTest");
