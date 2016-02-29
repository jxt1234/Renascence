#include "test/GPTest.h"
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
#include "frontend/GPFrontEndProducer.h"
#include "midend/GPMultiLayerTree.h"
#include "core/GPStreamFactory.h"
#include "evolution/GPEvolutionGroup.h"
using namespace std;
static int test_main()
{
    GPFunctionDataBase* base = GPFactory::createDataBase("func.xml", NULL);
    AUTOCLEAN(base);
    {
        GPProducer* sys = GPFactory::createProducer(base);
        AUTOCLEAN(sys);
        std::vector<const IStatusType*> inputs;
        GPPtr<GPFunctionTree> tree = sys->getFront()->vCreateFromFormula("S(TFR(C(C(x0,S(x1)),S(x1)), FR(x2, C(C(x0,S(x1)),S(x1)))))", inputs);
        GPPtr<GPMultiLayerTree> multiTree = new GPMultiLayerTree(tree.get());
        auto layers = multiTree->layers();
        for (auto iter : layers)
        {
            std::ostringstream outputstring;
            iter.second->render(outputstring);
            GPPRINT("%d:%s, ", iter.first, outputstring.str().c_str());
        }
        GPPRINT_FL("\n");
    }
    return 1;
}
class GPMultiLayerTreeTest:public GPTest
{
    public:
        virtual void run();
        GPMultiLayerTreeTest(){}
        virtual ~GPMultiLayerTreeTest(){}
};
void GPMultiLayerTreeTest::run()
{
    test_main();
}

static GPTestRegister<GPMultiLayerTreeTest> a("GPMultiLayerTreeTest");
