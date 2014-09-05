#include "test/GPTest.h"
#include "core/GPProducer.h"
#include "core/GPFactory.h"
#include "evolution/GPEvolutionGroup.h"
#include "utils/AutoClean.h"
#include <fstream>
#include <iostream>

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
                const IStatusType* bmp = base->queryType(string("TrBmp"));
                const IStatusType* doubleId = base->queryType(string("double"));
                vector<const IStatusType*> eOut;
                eOut.push_back(bmp);
                vector<const IStatusType*> eInp;
                GP_Input nullInput;
                GPEvolutionGroup* group = new GPEvolutionGroup(&gen, 10, 10);
                group->vSetInput(eInp);
                group->vSetOutput(eOut);
                group->vSetFixInput(nullInput);

                IGPAutoDefFunction* fit = gen.createFunction(doubleId, bmp);
                group->vEvolution(fit);
                fit->decRef();

                IGPAutoDefFunction* result = group->getBest();
                result->addRef();
                GPPRINT_FL("Best Fit is %f", group->getBestFit());

                delete group;
                ofstream outputF("output/result.xml");
                result->save(outputF);
                outputF.close();
                result->decRef();
            }
        }
        GPXmlEvolutionTest(){}
        virtual ~GPXmlEvolutionTest(){}
};

static GPTestRegister<GPXmlEvolutionTest> a("GPXmlEvolutionTest");
