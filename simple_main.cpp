#include "user/GPAPI.h"
#include <assert.h>
#include <fstream>
#include <iostream>
#include <string>
#include <fstream>
#include <iostream>
#include "core/GPFunctionDataBase.h"
#include "core/GPFactory.h"
#include "core/GPStreamFactory.h"
#include "core/GPProducer.h"
#include "producer/GPGraphicADF.h"
#include "core/GPStreamFactory.h"
using namespace std;
static int test_main()
{
    string formula = "C(S(I()), F(I()))";
    GPFunctionDataBase* base = GPFactory::createDataBase("func.xml", NULL);
    AUTOCLEAN(base);
    {
        GPProducer* sys = GPFactory::createProducer(base);
        {
            IGPAutoDefFunction* f = sys->vCreateFunctionFromFormula(formula);
            AUTOCLEAN(f);
            GPContents inp;
            GPContents* out = f->vRun(&inp);
            IGPAutoDefFunction* comp = sys->vCreateFunctionFromFormula("TrPackageFitCompute(x0)");
            auto _fits = comp->vRun(out);
            double* __fit = (double*)_fits->get(0);
            FUNC_PRINT_ALL(*__fit, f);
            _fits->clear();
            out->clear();
            delete _fits;
            delete out;
        }
    }
    return 1;
}
int main()
{
    GP_Set_Lib_Path("/Users/jiangxiaotang/Documents/Genetic-Program-Frame/");
    GP_Set_Stream_Path("/Users/jiangxiaotang/Documents/Genetic-Program-Frame/");
    {
        test_main();
    }
    return 0;
}

