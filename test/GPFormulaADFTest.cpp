#include "test/GPTest.h"
#include "core/GPProducer.h"
#include "frontend/GPFormulaTree.h"
#include <iostream>
#include "core/GPFactory.h"
#include "core/GPFunctionDataBase.h"
#include "core/GPStreamFactory.h"
#include "head.h"
using namespace std;
class GPFormulaADFTest:public GPTest
{
    public:
        virtual void run();
        GPFormulaADFTest(){}
        virtual ~GPFormulaADFTest(){}
};
void GPFormulaADFTest::run()
{
    GPFunctionDataBase* base = GPFactory::createDataBase("func.xml", NULL);
    const char* formula = "FIT(C(x1, ADF[filter, x1, x1, x1, C(x0, S(x0))]), x0)";
    AUTOCLEAN(base);
    {
        GPProducer* sys = GPFactory::createProducer(base);
        {
            const IStatusType* bmp_type = base->vQueryType("TrBmp");
            GPContents inputs;
            GPPtr<GPStreamWrap> input1 = GPStreamFactory::NewStream("input.jpg");
            inputs.push(bmp_type->vLoad(input1.get()), bmp_type);
            GPPtr<GPStreamWrap> input2 = GPStreamFactory::NewStream("output.jpg");
            inputs.push(bmp_type->vLoad(input2.get()), bmp_type);
            std::vector<const IStatusType*> inputTypes(2, bmp_type);
            IGPAutoDefFunction* comp = sys->createFunction(formula, inputTypes);
            auto _fits = comp->vRun(&inputs);
            double* __fit = (double*)_fits->get(0);
            FUNC_PRINT_ALL(*__fit, f);
            delete _fits;
        }
    }
}

static GPTestRegister<GPFormulaADFTest> a("GPFormulaADFTest");
