#include "test/GPTest.h"
#include "core/GPProducer.h"
#include "frontend/GPFormulaTree.h"
#include <iostream>
#include "core/GPFactory.h"
#include "core/GPFunctionDataBase.h"
#include "core/GPStreamFactory.h"
#include "head.h"
class GPMultiOutputTest:public GPTest
{
    public:
        virtual void run();
        GPMultiOutputTest(){}
        virtual ~GPMultiOutputTest(){}
};
void GPMultiOutputTest::run()
{
    GPFunctionDataBase* base = GPFactory::createDataBase("func.xml", NULL);
    const char* formula = "OUTPUT(FIT(C(x1, ADF[filter, x1, x1, x1, C(x0, S(x0))]), x0), FIT(x1, x0))";
    AUTOCLEAN(base);
    {
        GPPtr<GPProducer> sys = GPFactory::createProducer(base);
        GPPtr<GPProducer> sys2 = GPFactory::createProducer(base, GPFactory::STREAM);
        {
            const IStatusType* bmp_type = base->vQueryType("TrBmp");
            GPContents inputs;
            GPPtr<GPStreamWrap> input1 = GPStreamFactory::NewStream("input.jpg");
            inputs.push(bmp_type->vLoad(input1.get()), bmp_type);
            GPPtr<GPStreamWrap> input2 = GPStreamFactory::NewStream("output.jpg");
            inputs.push(bmp_type->vLoad(input2.get()), bmp_type);
            std::vector<const IStatusType*> inputTypes(2, bmp_type);
            {
                GPPtr<IGPAutoDefFunction> comp = sys->createFunction(formula, inputTypes);
                auto _fits = comp->vRun(&inputs);
                double* __fit1 = (double*)_fits->get(0);
                double* __fit2 = (double*)_fits->get(1);
                FUNC_PRINT_ALL(*__fit1, f);
                FUNC_PRINT_ALL(*__fit2, f);
                delete _fits;
            }
            {
                GPPtr<IGPAutoDefFunction> comp = sys2->createFunction(formula, inputTypes);
                auto _fits = comp->vRun(&inputs);
                double* __fit1 = (double*)_fits->get(0);
                double* __fit2 = (double*)_fits->get(1);
                FUNC_PRINT_ALL(*__fit1, f);
                FUNC_PRINT_ALL(*__fit2, f);
                delete _fits;
            }
        }
    }
}

static GPTestRegister<GPMultiOutputTest> a("GPMultiOutputTest");
