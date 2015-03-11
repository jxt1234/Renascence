#include "test/GPTest.h"
#include "core/GPFunctionDataBase.h"
#include "core/GPProducer.h"
#include "core/GPFactory.h"
#include "core/BasicComposeFunction.h"
#include <fstream>
#include <string>
#include <iostream>
#include "head.h"
using namespace std;
/*TODO*/
#if 0
class GPcombineTest:public GPTest
{
    public:
        virtual void run()
        {
            GPPtr<GPFunctionDataBase> base = GPFactory::createDataBase("func.xml", NULL);
            {
                GPProducer* sys = GPFactory::createProducer(base.get());
                AUTOCLEAN(sys);
                const IStatusType* bmp = base->vQueryType(string("TrBmp"));
                const IStatusType* doubleId = base->vQueryType(string("double"));
                const IStatusType* matrix = base->vQueryType(string("TrFilterMatrix"));
                /*Construct compose function*/
                vector<IGPAutoDefFunction*> funcs;
                GPContents bmpoutput;//For input
                GPContents bmpinput;
                IGPAutoDefFunction* c1 = sys->createFunction(matrix);
                funcs.push_back(c1);
                {
                    vector<const IStatusType*> input;
                    vector<const IStatusType*> output;
                    output.push_back(bmp);
                    vector<IGPAutoDefFunction*> fs = sys->vCreateAllFunction(output, input);
                    for (int i=0; i<fs.size(); ++i)
                    {
                        funcs.push_back(fs[i]);
                    }
                    GPASSERT(fs.size()>0);
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
                matrix->vSave(alloutput[0], cout);
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
        }
        GPcombineTest(){}
        virtual ~GPcombineTest(){}
};

static GPTestRegister<GPcombineTest> a("GPcombineTest");
#endif
