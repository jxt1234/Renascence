#include "user/GPAPI.h"
#include <assert.h>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int test_main()
{
#if 0
    ifstream soxml("/Users/jiangxiaotang/Documents/Genetic-Program-Frame/func.xml");
    auto producer = GP_Producer_Create(soxml, NULL, 0);
    /*Optimize*/
    {
        auto fitf = GP_Function_Create_ByType(producer, "double", "TrBmp", true);
        //GP_Function_Save(fitf, cout);
        auto fitfunction = [=](IGPAutoDefFunction* target){
            auto outputtype = GP_Function_Get_Outputs(target);
            GP_Input nullinput;
            auto output = GP_Function_Run(target, nullinput);
            GP_Input fitinput;
            fitinput.push_back(output[0]);
            auto foutput = GP_Function_Run(fitf, fitinput);
            double res = *(double*)(foutput[0]);
            output.clear();
            foutput.clear();
            return res;
        };
        /*Find Best, evolution group*/
        {
            auto bestf = GP_Function_CreateBest_ByType(producer, "TrBmp", "", true, fitfunction, 1000);
            cout << endl << "Best Fit is " << fitfunction(bestf) << endl;
            ofstream orzzz("output/GPAPI_Evolution.txt");
            GP_Function_Save(bestf, orzzz);
            GP_Function_Destroy(bestf);
        }
        GP_Function_Destroy(fitf);
    }
    GP_Producer_Destroy(producer);
#endif
    return 1;
}

int main()
{
    return test_main();
}
