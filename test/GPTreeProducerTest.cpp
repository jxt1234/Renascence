#include "test/GPTest.h"
#include "core/GPFunctionDataBase.h"
#include "core/GPProducer.h"
#include "core/GPFactory.h"
using namespace std;
class GPTreeProducerTest:public GPTest
{
    public:
        virtual void run();
        GPTreeProducerTest(){}
        virtual ~GPTreeProducerTest(){}
};
void GPTreeProducerTest::run()
{
    GPPtr<GPFunctionDataBase> base = GPFactory::createDataBase("func.xml", NULL);
    {
        GPProducer* sys = GPFactory::createProducer(base.get());
        AUTOCLEAN(sys);
        const IStatusType* bmp = base->vQueryType(string("TrBmp"));
        /*Construct compose function*/
        vector<IGPAutoDefFunction*> funcs;
        GPContents* bmpoutput;//For input
        vector<const IStatusType*> input;
        vector<const IStatusType*> output;
        output.push_back(bmp);
        vector<IGPAutoDefFunction*> fs = sys->vCreateAllFunction(output, input);
        GPASSERT(fs.size()>0);
        GPContents inp;
        IGPAutoDefFunction* fit = sys->vCreateFunctionFromFormula(string("TrPackageFitCompute(x0)"));
        int sum = 0;
        for (auto f : fs)
        {
            auto bmpoutput = f->vRun(&inp);
            auto fitoutput = fit->vRun(bmpoutput);
            double* __fit = (double*)fitoutput->get(0);
            FUNC_PRINT_ALL(*__fit, f);
            GPContents::destroy(bmpoutput);
            GPContents::destroy(fitoutput);
            sum++;
            if (sum >=100)
            {
                break;
            }
        }
        for (auto f : fs)
        {
            f->decRef();
        }
    }
}

static GPTestRegister<GPTreeProducerTest> a("GPTreeProducerTest");
