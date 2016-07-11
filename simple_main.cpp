#include "frontend/GPFormulaTree.h"
#include "frontend/GPFunctionFrontEndProducer.h"
#include "backend/GPBasicPiecesFunctionCreator.h"
#include "backend/GPSingleParallelMachine.h"
#include "core/GPFactory.h"
#include "core/GPProducer.h"
#include "core/GPFunctionTree.h"
#include <iostream>
#include <sstream>
#include "core/GPPieceFactory.h"
#include <fstream>
#include "core/GPStreamFactory.h"
#include "core/GPParallelMachineSet.h"
#include "user/GPAPI.h"
using namespace std;



static void __run()
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
    }}

#include "user/GPAPI.h"
static string gPath = "/Users/jiangxiaotang/Documents/Renascence/";

int main()
{
    GP_Set_Lib_Path(gPath.c_str());
    GP_Set_Stream_Path(gPath.c_str());
    //while(true)
    {
        __run();
    }
    return 1;
}
