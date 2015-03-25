#include "user/GPAPI.h"
#include <assert.h>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int test_main()
{
    ifstream soxml("/Users/jiangxiaotang/Documents/Genetic-Program-Frame/func.xml");
    GP_Set_Lib_Path("/Users/jiangxiaotang/Documents/Genetic-Program-Frame");
    auto producer = GP_Producer_Create(soxml, NULL, 0);
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
        GP_Function_Save(fitf, cout);
        cout.flush();
        /*Find Best, evolution group*/
        if (1)
        {
            auto bestf = GP_Function_CreateBest_ByType(producer, "TrBmp", "", true, fitfunction, 1000);
            cout << endl << "Best Fit is " << fitfunction(bestf) << endl;
            ofstream orzzz("/Users/jiangxiaotang/Documents/Genetic-Program-Frame/output/GPAPI_Evolution.txt");
            GP_Function_Save(bestf, orzzz);
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
