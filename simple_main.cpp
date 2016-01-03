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
    GPPtr<GPWStreamWrap> screen = GPStreamFactory::NewWStream(NULL, GPStreamFactory::USER);
    /*Single*/
    GPFunctionDataBase* base = GPFactory::createDataBase("func.xml", NULL);
    AUTOCLEAN(base);
    {
        GPProducer* sys = GPFactory::createProducer(base);
        GPProducer& gen = *sys;
        AUTOCLEAN(sys);
        const IStatusType* ist = base->vQueryType(string("TrFilterMatrix"));
        const IStatusType* bmp = base->vQueryType("TrBmp");
        vector<const IStatusType*> out;
        out.push_back(ist);
        vector<const IStatusType*> inp(2, bmp);
        IGPAutoDefFunction* f = gen.createFunction(out, inp);
        GPContents GPinp;
        GPPtr<GPStreamWrap> input = GPStreamFactory::NewStream("input.jpg");
        GPinp.push(bmp->vLoad(input.get()), bmp);
        input = GPStreamFactory::NewStream("output.jpg");
        GPinp.push(bmp->vLoad(input.get()), bmp);
        input = NULL;
        auto _output = f->vRun(&GPinp);
        GPASSERT(_output->size()==1);
        ist->vSave(_output->get(0), screen.get());
        cout <<endl;
        _output->clear();
        delete _output;
        f->decRef();
        /*Multi*/
        auto f_mul = gen.listAllFunction(out, inp, 1);
        GPASSERT(!f_mul.empty());
        cout <<"Multi function's size = "<< f_mul.size() << endl;
        for (int i=0; i<f_mul.size()&&i<10; ++i)
        {
            ostringstream fileName;
            fileName << "output/GPSearchIOTest/"<<i<<".xml";
            auto _output2 = f_mul[i]->vRun(&GPinp);
            GPASSERT(_output2->size()==1);
            ist->vSave(_output2->get(0), screen.get());
            cout << endl;
            _output2->clear();
            delete _output2;
            GPPtr<GPWStreamWrap> output = GPStreamFactory::NewWStream(fileName.str().c_str());
            GPPtr<GPTreeNode> n = f_mul[i]->vSave();
            xmlReader::dumpNodes(n.get(), output.get());
        }
        GPinp.clear();
    }
    return 1;
}


int main()
{
    GP_Set_Stream_Path("/Users/jiangxiaotang/Documents/Genetic-Program-Frame/");
    GP_Set_Lib_Path("/Users/jiangxiaotang/Documents/Genetic-Program-Frame/");
    return test_main();
}