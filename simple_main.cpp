#include "user/GPAPI.h"
#include "core/GPProducer.h"
#include "frontend/GPFormulaTree.h"
#include <iostream>
#include "core/GPFactory.h"
#include "core/GPFunctionDataBase.h"
#include "head.h"
using namespace std;
#include <fstream>
#include <iostream>
#include "core/GPFunctionDataBase.h"
#include "core/GPFactory.h"
#include "core/GPStreamFactory.h"
#include "core/GPProducer.h"
#include "backend/GPGraphicADF.h"
using namespace std;

static int test_main()
{
    //string formula = "TrPackageCompse(TrPackageSaturation(TrPackageInput()), TrPackageFilterMatrix(TrPackageInput()))";
    string formula = "C(S(I()), F(I()))";
    GPFunctionDataBase* base = GPFactory::createDataBase("func.xml", NULL);
    AUTOCLEAN(base);
    {
        GPProducer* sys = GPFactory::createProducer(base);
        {
            IGPAutoDefFunction* f = sys->createFunction(formula);
            AUTOCLEAN(f);
            GPContents inp;
            GPContents* out = f->vRun(&inp);
            IGPAutoDefFunction* comp = sys->createFunction("TrPackageFitCompute(ADF(TrBmp:x0, TrBmp:x0, TrBmp:x0))");
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
    GP_Set_Stream_Path("/Users/jiangxiaotang/Documents/Genetic-Program-Frame/");
    GP_Set_Lib_Path("/Users/jiangxiaotang/Documents/Genetic-Program-Frame/");
    return test_main();
}