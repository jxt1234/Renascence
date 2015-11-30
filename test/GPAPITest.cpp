#include "test/GPTest.h"
#include "core/GPStreamFactory.h"
#include "user/GPAPI.h"
#include "user/GPContents.h"
#include <assert.h>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

struct OptMeta
{
    IGPAutoDefFunction* pFit;
};

static double _OptFunction(IGPAutoDefFunction* target, void* meta)
{
    OptMeta* _meta = (OptMeta*)meta;
    GPContents nullInput;
    auto output = GP_Function_Run(target, &nullInput);
    auto foutput = GP_Function_Run(_meta->pFit, output);
    double res = *(double*)(foutput->get(0));
    output->clear();
    GPContents::destroy(output);
    GPContents::destroy(foutput);
    return res;
}

static int test_main()
{
    GPPtr<GPStreamWrap> soxml = GPStreamFactory::NewStream("func.xml", GPStreamFactory::FILE);
    IFunctionTable* tables = NULL;
    GPStream* streamTables = soxml.get();
    auto producer = GP_Producer_Create(&streamTables, &tables, 1, 0);
    GPPtr<GPWStreamWrap> screen = GPStreamFactory::NewWStream(NULL, GPStreamFactory::USER);
    /*Input and output*/
    {
        auto adf = GP_Function_Create_ByType(producer, "TrFilterMatrix", "", NULL);
        {
            GPPtr<GPWStreamWrap> output = GPStreamFactory::NewWStream("output/GPAPI_base.txt");
            GP_Function_Save(adf, output.get());
        }
        GP_Function_Destroy(adf);
        GPPtr<GPStreamWrap> input = GPStreamFactory::NewStream("output/GPAPI_base.txt");
		auto adf2 = GP_Function_Create_ByStream(producer, input.get());
        {
            GPPtr<GPWStreamWrap> output = GPStreamFactory::NewWStream("output/GPAPI_base2.txt");
            GP_Function_Save(adf2, output.get());
        }
		GP_Function_Destroy(adf2);
    }
    /*Formula*/
    {
        string formula = "C(S(I()), F(I()))";
        auto adf = GP_Function_Create_ByFormula(producer, formula.c_str(), NULL);
        GPPtr<GPWStreamWrap> output = GPStreamFactory::NewWStream("output/GPAPI_Formula.txt");
        GP_Function_Save(adf, output.get());
        GP_Function_Destroy(adf);
    }
    /*Run*/
    {
        auto adf = GP_Function_Create_ByType(producer, "TrFilterMatrix", "", NULL);
        GPContents gp_inputs;
        auto gp_output = GP_Function_Run(adf, &gp_inputs);
        assert(1==gp_output->size());
        auto unit = gp_output->contents[0];
        unit.type->vSave(unit.content, screen.get());
        GPContents::destroy(gp_output);
        GP_Function_Destroy(adf);
    }
    /*Optimize*/
    {
        auto fitf = GP_Function_Create_ByType(producer, "double", "TrBmp", NULL);
        OptMeta meta;
        meta.pFit = fitf;
        GPOptimizorInfo optinfo;
        optinfo.pMeta = &meta;
        optinfo.pFitComputeFunction = _OptFunction;
        optinfo.nMaxADFDepth = 10;
        optinfo.nMaxRunTimes = 1000;
        optinfo.nOptimizeType = 0;
        /*Single Opt*/
        {
            string formula = "C(S(I()), F(I()))";
            auto adf  = GP_Function_Create_ByFormula(producer, formula.c_str(), NULL);
            optinfo.nMaxRunTimes = 100;
            GP_Function_Optimize(adf, &optinfo);
            cout << _OptFunction(adf, &meta) << endl;
            GPPtr<GPWStreamWrap> output = GPStreamFactory::NewWStream("output/GPAPI_Formula_SOpt.txt");
            GP_Function_Save(adf, output.get());
            GP_Function_Destroy(adf);
        }
        /*Find Best, evolution group*/
        {
            auto bestf = GP_Function_Create_ByType(producer, "TrBmp", "", &optinfo);
            cout << _OptFunction(bestf, &meta) << endl;
            optinfo.nMaxRunTimes = 10;
            GPPtr<GPWStreamWrap> output = GPStreamFactory::NewWStream("output/GPAPI_Evolution.txt");
            GP_Function_Save(bestf, output.get());
            GP_Function_Destroy(bestf);
        }
        GP_Function_Destroy(fitf);
    }
    GP_Producer_Destroy(producer);
    return 1;
}

class GPAPITest:public GPTest
{
    public:
        virtual void run();
        GPAPITest(){}
        virtual ~GPAPITest(){}
};
void GPAPITest::run()
{
    test_main();
}

static GPTestRegister<GPAPITest> a("GPAPITest");

