#include "test/GPTest.h"
#include "system/xmlGenerateSystem.h"
#include "evolution/GPEvolutionGroup.h"
#include <fstream>
#include <iostream>

using namespace std;

class GPXmlEvolutionTest:public GPTest
{
    public:
        virtual void run()
        {
            xmlGenerateSystem gen;
            gen.addXml("funcV2.xml", NULL, &cout);
            int bmpId = gen.queryType(string("TrBmp"));
            int doubleId = gen.queryType(string("double"));
            vector<int> eOut;
            eOut.push_back(bmpId);
            vector<int> eInp;
            GP_Input nullInput;
            GPEvolutionGroup* group = new GPEvolutionGroup(&gen, 10, 2);
            group->vSetInputId(eInp);
            group->vSetOutputId(eOut);
            group->vSetFixInput(nullInput);

            IGPAutoDefFunction* fit = gen.createFunction(doubleId, bmpId);
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
        GPXmlEvolutionTest(){}
        virtual ~GPXmlEvolutionTest(){}
};

static GPTestRegister<GPXmlEvolutionTest> a("GPXmlEvolutionTest");
