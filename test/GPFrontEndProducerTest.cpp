#include "test/GPTest.h"
#include "frontend/GPFunctionFrontEndProducer.h"
#include "core/GPFactory.h"
class GPFrontEndProducerTest:public GPTest
{
    public:
        virtual void run();
        GPFrontEndProducerTest(){}
        virtual ~GPFrontEndProducerTest(){}
};

static const char* gFormulas[] = {
    "FIT(C(x1, ADF[filter, x1, x1, x1, C(x0, S(x0))]), x0)",
    "C(x0, S(S(x0)))"
};
void GPFrontEndProducerTest::run()
{
    GPFunctionDataBase* base = GPFactory::createDataBase("func.xml", NULL);
    AUTOCLEAN(base);
    int l = sizeof(gFormulas)/sizeof(const char*);
    for (int i=0; i<l; ++i)
    {
        const char* formula = gFormulas[i];
        GPPtr<GPFunctionFrontEndProducer> producer = new GPFunctionFrontEndProducer(base);
        GPPtr<GPFunctionTree> tree = producer->vCreateFromFormula(formula, std::vector<const IStatusType*>(2, base->vQueryType("TrBmp")));
        FUNC_PRINT_ALL(formula, s);
        FUNC_PRINT_ALL(tree->dump().c_str(), s);
    }
}

static GPTestRegister<GPFrontEndProducerTest> a("GPFrontEndProducerTest");
