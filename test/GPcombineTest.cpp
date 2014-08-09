#include "test/GPTest.h"
#include "core/GPProducer.h"
#include "core/GPProducerFactory.h"
#include "core/BasicComposeFunction.h"
#include "utils/AutoClean.h"
#include <fstream>
#include <string>
#include <iostream>
#include <assert.h>
using namespace std;

class GPcombineTest:public GPTest
{
    public:
        virtual void run()
        {
            GPProducer* sys = GPProducerFactory::create("func.xml", NULL);
            AUTOCLEAN(sys);
            int bmpId = sys->queryType(string("TrBmp"));
            const IStatusType& bmp = sys->queryType(bmpId);
            int doubleId = sys->queryType(string("double"));
            int matrixId = sys->queryType(string("TrFilterMatrix"));
            const IStatusType& matrix = sys->queryType(matrixId);


            /*Construct compose function*/
            vector<IGPAutoDefFunction*> funcs;
            GP_Output bmpoutput;//For input
            GP_Input bmpinput;
            IGPAutoDefFunction* c1 = sys->createFunction(matrixId);
            funcs.push_back(c1);
            {
                vector<int> input;
                vector<int> output;
                output.push_back(bmpId);
                vector<IGPAutoDefFunction*> fs = sys->vCreateAllFunction(output, input);
                for (int i=0; i<fs.size(); ++i)
                {
                    funcs.push_back(fs[i]);
                }
                assert(fs.size()>0);
                GP_Input inp;
                bmpoutput = fs[0]->run(inp);
                bmpinput.push_back(bmpoutput[0]);
            }
            IGPAutoDefFunction* fit = sys->vCreateFunctionFromName(string("TrPackageFitCompute"));
            funcs.push_back(fit);

            FUNC_PRINT(funcs.size());

            IGPAutoDefFunction* compf = new GPCombineADF(funcs);
            for (int i=0; i<funcs.size(); ++i)
            {
                funcs[i]->decRef();
            }

            GP_Output alloutput = compf->run(bmpinput);
            FUNC_PRINT(alloutput.size());
            {
                ofstream os("output/GPcombineTest.xml");
                compf->save(os);
                os.close();
            }


            //The first is TrFilterMatrix
            matrix.print(cout, alloutput[0]);
            cout << endl;
            //Mids are all bmps
            for (int i=1; i<alloutput.size()-1; ++i)
            {
                GP_Input inp;
                inp.push_back(alloutput[i]);
                GP_Output fit_bmp = fit->run(inp);
                double* __fit = (double*)fit_bmp[0];
                FUNC_PRINT_ALL(*__fit, f);
                fit_bmp.clear();
            }
            double* __fit = (double*)alloutput[alloutput.size()-1];
            FUNC_PRINT_ALL(*__fit, f);

            alloutput.clear();
            bmpoutput.clear();
            compf->decRef();
        }
        GPcombineTest(){}
        virtual ~GPcombineTest(){}
};

static GPTestRegister<GPcombineTest> a("GPcombineTest");
