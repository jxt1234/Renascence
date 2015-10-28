#include "test/GPTest.h"
#include "frontend/GPFormulaTree.h"
#include <iostream>
#include "core/GPFactory.h"
#include "core/GPFunctionDataBase.h"
#include "head.h"
using namespace std;
class GPFormulaTest:public GPTest
{
    public:
        virtual void run();
        GPFormulaTest(){}
        virtual ~GPFormulaTest(){}
};
void GPFormulaTest::run()
{
    //string formula = "TrPackageCompse(TrPackageSaturation(TrPackageInput()), TrPackageFilterMatrix(TrPackageInput()))";
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
}

static GPTestRegister<GPFormulaTest> a("GPFormulaTest");
