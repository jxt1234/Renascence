#include "math/FormulaTree.h"
#include <iostream>
#include "core/GPProducer.h"
#include "core/GPFactory.h"
#include "core/GPFunctionDataBase.h"
#include "head.h"
using namespace std;

int main()
{
    string formula = "TrPackageCompse(TrPackageSaturation(TrPackageInput()), TrPackageFilterMatrix(TrPackageInput()))";
    GPFunctionDataBase* base = GPFactory::createDataBase("func.xml", NULL);
    AUTOCLEAN(base);
    {
        GPProducer* sys = GPFactory::createProducer(base);
        {
            IGPAutoDefFunction* f = sys->vCreateFunctionFromFormula(formula);
            AUTOCLEAN(f);
            GP_Input inp;
            GP_Output out = f->run(inp);
            inp.push_back(out[0]);
            IGPAutoDefFunction* comp = sys->vCreateFunctionFromName("TrPackageFitCompute");
            AUTOCLEAN(comp);
            GP_Output _fits = comp->run(inp);
            double* __fit = (double*)_fits[0];
            FUNC_PRINT_ALL(*__fit, f);
            out.clear();
            _fits.clear();
        }
    }
    return 1;
}
