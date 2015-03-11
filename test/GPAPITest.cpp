#include "test/GPTest.h"
#include "user/GPAPI.h"
#include <assert.h>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

static int test_main()
{
    ifstream soxml("func.xml");
    auto producer = GP_Producer_Create(soxml, NULL, 0);
    /*Input and output*/
    {
        auto adf = GP_Function_Create_ByType(producer, "TrFilterMatrix", "", true);
        ofstream output("output/GPAPI_base.txt");
        GP_Function_Save(adf, output);
        GP_Function_Destroy(adf);
		ifstream input("output/GPAPI_base.txt");
		auto adf2 = GP_Function_Create_ByStream(producer, input);
		ofstream output2("output/GPAPI_base2.txt");
        GP_Function_Save(adf2, output2);
		GP_Function_Destroy(adf2);
    }
    /*Formula*/
    {
        string formula = "TrPackageCompse(TrPackageSaturation(TrPackageInput()), TrPackageFilterMatrix(TrPackageInput()))";
        auto adf = GP_Function_Create_ByFormula(producer, formula.c_str());
        ofstream output("output/GPAPI_Formula.txt");
        GP_Function_Save(adf, output);
        GP_Function_Destroy(adf);
    }
    /*Run*/
    {
        auto adf = GP_Function_Create_ByType(producer, "TrFilterMatrix", "", true);
        GPTYPES inputs, outputs;
        GP_Function_Get_Inputs(adf, &inputs);
        assert(0 == inputs.size());
        GP_Function_Get_Outputs(adf, &outputs);
        assert(1==outputs.size());
        GPContents gp_inputs;
        auto gp_output = GP_Function_Run(adf, &gp_inputs);
        assert(1==gp_output->size());
        auto unit = gp_output->contents[0];
        unit.type->vSave(unit.content, cout);
        GPContents::destroy(gp_output);
        GP_Function_Destroy(adf);
    }
    /*Optimize*/
    {
        auto fitf = GP_Function_Create_ByType(producer, "double", "TrBmp", true);
        auto fitfunction = [=](IGPAutoDefFunction* target){
            GPContents nullinput;
            auto output = GP_Function_Run(target, &nullinput);
            auto foutput = GP_Function_Run(fitf, output);
            double res = *(double*)(foutput->get(0));
            output->clear();
            GPContents::destroy(output);
            GPContents::destroy(foutput);
            return res;
        };
        /*Single Opt*/
        {
            string formula = "TrPackageCompse(TrPackageSaturation(TrPackageInput()), TrPackageFilterMatrix(TrPackageInput()))";
            auto adf  = GP_Function_Create_ByFormula(producer, formula.c_str());
            GP_Function_Optimize(adf, fitfunction, 1, 10);
            cout << fitfunction(adf) << endl;
            ofstream orzzz("output/GPAPI_Formula_SOpt.txt");
            GP_Function_Save(adf, orzzz);
            GP_Function_Destroy(adf);
        }
        /*Find Best, evolution group*/
        {
            auto bestf = GP_Function_CreateBest_ByType(producer, "TrBmp", "", true, fitfunction, 1000);
            cout << fitfunction(bestf) << endl;
            ofstream orzzz("output/GPAPI_Evolution.txt");
            GP_Function_Save(bestf, orzzz);
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

