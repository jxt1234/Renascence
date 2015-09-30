#include "test/GPTest.h"
#include "core/GPProducer.h"
#include "core/GPFactory.h"
#include "evolution/GPEvolutionGroup.h"
#include "utils/AutoClean.h"
#include <fstream>
#include <iostream>
#include "core/GPStreamFactory.h"

using namespace std;

class GPXmlEvolutionTest:public GPTest
{
    public:
        virtual void run()
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
                GPEvolutionGroup* group = new GPEvolutionGroup(&gen, 2, 2);
                group->vSetInput(eInp);
                group->vSetOutput(eOut);
                IGPAutoDefFunction* fit = gen.createFunction(doubleId, bmp);
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
        }
        GPXmlEvolutionTest(){}
        virtual ~GPXmlEvolutionTest(){}
};

static GPTestRegister<GPXmlEvolutionTest> a("GPXmlEvolutionTest");
