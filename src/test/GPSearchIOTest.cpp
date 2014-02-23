#include "test/GPTest.h"
#include "system/xmlGenerateSystem.h"
#include <iostream>
#include <assert.h>
using namespace std;
class GPSearchIOTest:public GPTest
{
    public:
        virtual void run()
        {
            xmlGenerateSystem gen;
            gen.addXml("func.xml", NULL);
            int outId = gen.queryType(string("TrFilterMatrix"));
            const IStatusType& ist = gen.queryType(outId);
            vector<int> out;
            out.push_back(outId);
            vector<int> inp;
            IGPAutoDefFunction* f = gen.vCreateFunction(out, inp);
            vector<void*> GPinp;
            GP_Output GPout = f->run(GPinp);
            vector<void*> _output;
            GP_Output_collect(_output, GPout);
            assert(_output.size()==1);
            ist.print(cout, _output[0]);
            GP_Output_clear(GPout);

            delete f;
        }
        GPSearchIOTest(){}
        virtual ~GPSearchIOTest(){}
};

static GPTestRegister<GPSearchIOTest> a("GPSearchIOTest");
