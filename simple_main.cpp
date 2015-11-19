#include "user/GPAPI.h"
#include <assert.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "core/GPFunctionDataBase.h"
#include "core/GPFactory.h"
#include "core/GPStreamFactory.h"
#include "core/GPProducer.h"
#include "backend/GPGraphicADF.h"
#include "core/GPStreamFactory.h"
#include "evolution/GPEvolutionGroup.h"
using namespace std;
static int test_main()
{
    GPFunctionDataBase* base = GPFactory::createDataBase("func.xml", NULL);
    AUTOCLEAN(base);
    {
        GPProducer* sys = GPFactory::createProducer(base);
        GPProducer& gen = *sys;
        AUTOCLEAN(sys);
        const IStatusType* bmp = base->vQueryType(string("TrBmp"));
        const IStatusType* doubleId = base->vQueryType(string("double"));
        vector<const IStatusType*> eOut;
        eOut.push_back(bmp);
        vector<const IStatusType*> eInp;
        GPEvolutionGroup* group = new GPEvolutionGroup(&gen, 200, 200);
        group->vSetInput(eInp);
        group->vSetOutput(eOut);
        IGPAutoDefFunction* fit = gen.createFunction(vector<const IStatusType*>(1,doubleId), vector<const IStatusType*>(1,bmp));
        auto fitfunc = [&](IGPAutoDefFunction* f){
            GPContents nullinput;
            GPContents* result = f->vRun(&nullinput);
            GPContents* fits = fit->vRun(result);
            double fitresult = *(double*)fits->get(0);
            result->clear();
            delete result;
            fits->clear();
            delete fits;
            return fitresult;
        };
        group->vEvolutionFunc(fitfunc);
        fit->decRef();
        
        GPPtr<IGPAutoDefFunction> result = group->getBest();
        GPPRINT_FL("Best Fit is %f", group->getBestFit());
        
        delete group;
        GPPtr<GPWStreamWrap> outputF = GPStreamFactory::NewWStream("output/tree_result.xml");
        GPPtr<GPTreeNode> n = result->vSave();
        xmlReader::dumpNodes(n.get(), outputF.get());
    }
    return 1;
}

int main()
{
    GP_Set_Lib_Path("/Users/jiangxiaotang/Documents/Genetic-Program-Frame/");
    GP_Set_Stream_Path("/Users/jiangxiaotang/Documents/Genetic-Program-Frame/");
    {
        test_main();
    }
    return 0;
}

