#include "user/GPAPI.h"
#include "core/GPProducer.h"
#include "frontend/GPFormulaTree.h"
#include <iostream>
#include "core/GPFactory.h"
#include "core/GPFunctionDataBase.h"
#include "head.h"
#include <fstream>
#include <iostream>
#include "core/GPFunctionDataBase.h"
#include "core/GPFactory.h"
#include "core/GPStreamFactory.h"
#include "core/GPProducer.h"
#include "frontend/GPFrontEndProducer.h"
#include "backend/GPGraphicADF.h"
#include "midend/GPMultiLayerTree.h"
#include "evolution/GPEvolutionGroup.h"
#include <sstream>
using namespace std;

struct OptMeta
{
    IGPAutoDefFunction* pFit;
    AGPContents* pInput;
    AGPContents* pOutput;
};

static double _OptFunction(IGPAutoDefFunction* target, void* meta)
{
    OptMeta* _meta = (OptMeta*)meta;
    AGPContents* output = GP_Function_Run(target, _meta->pInput);
    AGPContents* totalInput = GP_Contents_CreateCollector();
    GP_Contents_Collect(totalInput, output, 0);
    GP_Contents_Collect(totalInput, _meta->pOutput, 0);
    auto foutput = GP_Function_Run(_meta->pFit, totalInput);
    double res = *(double*)(GP_Contents_Get(foutput, 0));
    GP_Contents_Destroy(totalInput);
    GP_Contents_Destroy(output);
    GP_Contents_Destroy(foutput);
    return res;
}




static int test_main()
{
    GPPtr<GPStreamWrap> soxml = GPStreamFactory::NewStream("func.xml", GPStreamFactory::FILE);
    IFunctionTable* tables = NULL;
    GPStream* streamTables = soxml.get();
    auto producer = GP_Producer_Create(&streamTables, &tables, 1, 0);
    GPPtr<GPWStreamWrap> screen = GPStreamFactory::NewWStream(NULL, GPStreamFactory::USER);
    /*Optimize*/
    {
        auto fitf = GP_Function_Create_ByFormula(producer, "FIT(x0,x1)", "TrBmp TrBmp", NULL);
        OptMeta meta;
        meta.pFit = fitf;
        GPStream* input[2];
        input[0] = GP_Stream_Create("input.jpg");
        input[1] = GP_Stream_Create("input_test_simple.jpg");
        meta.pInput = GP_Contents_Load(producer, input, "TrBmp TrBmp", 2);
        GP_Stream_Destroy(input[0]);
        GP_Stream_Destroy(input[1]);
        input[0] = GP_Stream_Create("output.jpg");
        meta.pOutput = GP_Contents_Load(producer, input, "TrBmp", 1);
        GP_Stream_Destroy(input[0]);
        
        GPOptimizorInfo optinfo;
        optinfo.pMeta = &meta;
        optinfo.pFitComputeFunction = _OptFunction;
        optinfo.nMaxADFDepth = 10;
        optinfo.nMaxRunTimes = 1000;
        optinfo.nOptimizeType = 0;
        /*Find Best, ADF test*/
        {
            auto bestf = GP_Function_Create_ByFormula(producer, "C(x1, ADF[filter, x1, x1, x1, C(x0, S(x0))])", "TrBmp TrBmp", &optinfo);
            cout << "ADF" << _OptFunction(bestf, &meta) << endl;
            optinfo.nMaxRunTimes = 1000000;
            AGPStrings* s = GP_Function_GetFormula(bestf, "filter");
            int n = GP_Strings_Number(s);
            for (int i=0; i<n; ++i)
            {
                FUNC_PRINT_ALL(GP_Strings_Get(s, i), s);
            }
            GP_Strings_Free(s);
            GPPtr<GPWStreamWrap> output = GPStreamFactory::NewWStream("output/GPAPI_EvolutionADF.txt");
            GP_Function_Save(bestf, output.get());
            GP_Function_Destroy(bestf);
        }
        GP_Function_Destroy(fitf);
        GP_Contents_Destroy(meta.pOutput);
        GP_Contents_Destroy(meta.pInput);
    }
    GP_Producer_Destroy(producer);
    return 1;
}


int main()
{
    GP_Set_Stream_Path("/Users/jiangxiaotang/Documents/Genetic-Program-Frame/");
    GP_Set_Lib_Path("/Users/jiangxiaotang/Documents/Genetic-Program-Frame/");
    return test_main();
}