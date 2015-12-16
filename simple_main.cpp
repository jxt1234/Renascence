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
    GPContents* pMeta;
};

static double _OptFunction(IGPAutoDefFunction* target, void* meta)
{
    OptMeta* _meta = (OptMeta*)meta;
    auto output = GP_Function_Run(target, _meta->pMeta);
    GPContents totalInput;
    totalInput.push(output->getContent(0));
    totalInput.push(_meta->pMeta->getContent(1));
    auto foutput = GP_Function_Run(_meta->pFit, &totalInput);
    double res = *(double*)(foutput->get(0));
    output->clear();
    GPContents::destroy(output);
    GPContents::destroy(foutput);
    return res;
}


static int test_main()
{
    GPFunctionDataBase* base = GPFactory::createDataBase("func.xml", NULL);
    AUTOCLEAN(base);
    {
        GPProducer* sys = GPFactory::createProducer(base);
        GPProducer& gen = *sys;
        AUTOCLEAN(sys);
        const IStatusType* bmp = base->vQueryType(string("TrBmp"));
        vector<const IStatusType*> eOut(1, bmp);
        vector<const IStatusType*> eInp(3, bmp);
        
        
        GPEvolutionGroup* group = new GPEvolutionGroup(&gen, 20, 20);
        group->vSetInput(eInp);
        group->vSetOutput(eOut);
        GPContents inp;
        GPPtr<GPStreamWrap> inputStream = GPStreamFactory::NewStream("input.jpg");
        inp.push(bmp->vLoad(inputStream.get()), NULL);
        inputStream = GPStreamFactory::NewStream("input_sharp.jpg");
        inp.push(bmp->vLoad(inputStream.get()), NULL);
        inputStream = GPStreamFactory::NewStream("input_test_simple.jpg");
        inp.push(bmp->vLoad(inputStream.get()), NULL);
        
        GPContents target;
        inputStream = GPStreamFactory::NewStream("output.jpg");
        target.push(bmp->vLoad(inputStream.get()), NULL);
        inputStream = NULL;
        
        IGPAutoDefFunction* fit = gen.createFunction("FIT(x0, x1)", vector<const IStatusType*>(2,bmp));
        
        
        auto fitfunc = [&](IGPAutoDefFunction* f){
            GPContents* result = f->vRun(&inp);
            GPContents temp;
            temp.push(result->getContent(0));
            temp.push(target.getContent(0));
            GPContents* fits = fit->vRun(&temp);
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
        inp.clear();
        target.clear();
    }
    return 1;
}


int main()
{
    GP_Set_Stream_Path("/Users/jiangxiaotang/Documents/Genetic-Program-Frame/");
    GP_Set_Lib_Path("/Users/jiangxiaotang/Documents/Genetic-Program-Frame/");
    return test_main();
}