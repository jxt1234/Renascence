#include "test/GPTest.h"
#include "frontend/GPFormulaTree.h"
#include "frontend/GPFunctionFrontEndProducer.h"
#include "core/GPFunctionTree.h"
#include <iostream>
#include <fstream>
#include "core/GPStreamFactory.h"
#include "platform/system_lib.h"


static const char* gFormulaNew =
"REDUCE((MAP((x0, x1), S(x0, x1), [[a0, a1], [b0, b1]]->[a0,b1,a1], a1==b0)), S(x0, x1), [a0, a1, a2]->[a0, a1])";

class GPParallelTest:public GPTest
{
    public:
        virtual void run();
        GPParallelTest(){}
        virtual ~GPParallelTest(){}
};
void GPParallelTest::run()
{
    GPFormulaTree tree;
    tree.setFormula(gFormulaNew);
    {
        std::ofstream outputfile("output/GPParallelTest.xml");
        tree.root()->render(outputfile);
    }
    GPPtr<GPFunctionDataBase> base = new GPFunctionDataBase();
    {
        GPPtr<GPStream> input = GPStreamFactory::NewStream("func.xml");
        base->loadXml(input.get());
    }
    {
        GPPtr<GPFunctionFrontEndProducer> producer = new GPFunctionFrontEndProducer(base.get());
        GPPtr<GPFunctionTree> tree = producer->vCreateFromFormula(gFormulaNew, std::vector<const IStatusType*>());
        FUNC_PRINT_ALL(tree->dump().c_str(), s);
    }
}

static GPTestRegister<GPParallelTest> a("GPParallelTest");
