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
                const IStatusType* matrix = base->vQueryType(string("TrFilterMatrix"));
                /*Construct compose function*/
                vector<IGPAutoDefFunction*> funcs;
                GPContents* bmpoutput;//For input
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
                    GPContents inp;
                    bmpoutput = fs[0]->vRun(&inp);
                }
                IGPAutoDefFunction* fit = sys->vCreateFunctionFromName(string("TrPackageFitCompute"));
                funcs.push_back(fit);

                IGPAutoDefFunction* compf = new GPCombineADF(funcs);
                for (int i=0; i<funcs.size(); ++i)
                {
                    funcs[i]->decRef();
                }
                FUNC_PRINT(bmpoutput->size());
                {
                    ofstream os("output/GPcombineTest.xml");
                    compf->vSave(os);
                    os.close();
                }
                GPContents* alloutput = compf->vRun(bmpoutput);
                FUNC_PRINT(alloutput->size());


                //The first is TrFilterMatrix
                auto unit = (*alloutput)[0];
                unit.type->vSave(unit.content, cout);
                cout << endl;
                //Mids are all bmps
                for (int i=1; i<alloutput->size()-1; ++i)
                {
                    FUNC_PRINT(i);
                    GPContents inp;
                    inp.push((*alloutput)[i]);
                    FUNC_PRINT_ALL((*alloutput)[i].type->name().c_str(),s);
                    GPContents* fit_bmp = fit->vRun(&inp);
                    double* __fit = (double*)fit_bmp->get(0);
                    FUNC_PRINT_ALL(*__fit, f);
                    GPContents::destroy(fit_bmp);
                }
                double* __fit = (double*)alloutput->get(alloutput->size()-1);
                FUNC_PRINT_ALL(*__fit, f);

                GPContents::destroy(alloutput);
                GPContents::destroy(bmpoutput);

                compf->decRef();
            }
        }
        GPcombineTest(){}
        virtual ~GPcombineTest(){}
};

static GPTestRegister<GPcombineTest> a("GPcombineTest");
