#include "user/GPAPI.h"
#include <assert.h>
#include <fstream>
#include <iostream>
#include <string>

/*TODO*/
#include "core/GPStreamFactory.h"
using namespace std;

int test_main()
{
    const char* soxmlname = "/Users/jiangxiaotang/Documents/Genetic-Program-Frame/func.xml";
    GPPtr<GPStreamWrap> soxml = GPStreamFactory::NewStream(soxmlname, GPStreamFactory::FILE);
    GPPtr<GPWStreamWrap> screenstream = GPStreamFactory::NewWStream(NULL, GPStreamFactory::USER);
    GP_Set_Lib_Path("/Users/jiangxiaotang/Documents/Genetic-Program-Frame");
    auto producer = GP_Producer_Create(soxml.get(), NULL, 0);
    /*Optimize*/
    {
        auto fitf = GP_Function_Create_ByType(producer, "double", "TrBmp", false);
        //GP_Function_Save(fitf, cout);
        auto fitfunction = [=](IGPAutoDefFunction* target){
            vector<const IStatusType*> outputtype;
            GP_Function_Get_Outputs(target, &outputtype);
            GPContents nullinput;
            auto output = GP_Function_Run(target, &nullinput);
            auto foutput = GP_Function_Run(fitf, output);
            double res = *(double*)(foutput->get(0));
            GPContents::destroy(output);
            GPContents::destroy(foutput);
            return res;
        };
        GP_Function_Save(fitf, screenstream.get());
        /*Find Best, evolution group*/
        if (1)
        {
            auto bestf = GP_Function_CreateBest_ByType(producer, "TrBmp", "", true, fitfunction, 1000);
            cout << endl << "Best Fit is " << fitfunction(bestf) << endl;
            GPPtr<GPWStreamWrap> orzzz = GPStreamFactory::NewWStream("/Users/jiangxiaotang/Documents/Genetic-Program-Frame/output/GPAPI_Evolution.txt", GPStreamFactory::FILE);
            GP_Function_Save(bestf, orzzz.get());
            GP_Function_Destroy(bestf);
        }
        GP_Function_Destroy(fitf);
    }
    GP_Producer_Destroy(producer);
    return 1;
}

int main()
{
    return test_main();
}
