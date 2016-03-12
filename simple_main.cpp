#include "frontend/GPFormulaTree.h"
#include "frontend/GPFunctionFrontEndProducer.h"
#include "core/GPFunctionTree.h"
#include <iostream>
#include <fstream>
#include "core/GPStreamFactory.h"
#include "platform/system_lib.h"

static const char* gFormula = "TrPackageSaturation(TrPackageFilterTransformFromRegress(TrPackageCompse(TrPackageCompse(x0,TrPackageSaturation(x1)),TrPackageSaturation(x3)), TrPackageFilterMatrixRegress(x2, TrPackageCompse(TrPackageCompse(x0,TrPackageSaturation(x1)),ADF[Input, x5]))))";

static const char* gFormulaNew =
"REDUCE((MAP((x0, x1), S(x0, x1), [[a0, a1], [b0, b1]]->[a0,b1,a1], a1==b0)), S(x0, x1), [a0, a1, a2]->[a0, a1])";


static const char* gPath = "/Users/jiangxiaotang/Documents/Renascence/";

int main()
{
    GPStreamFactory::setParentPath(gPath);
    system_set_path(gPath);
    GPFormulaTree tree;
    tree.setFormula(gFormulaNew);
    {
        std::ofstream outputfile("/Users/jiangxiaotang/Documents/Renascence/.test.xml");
        tree.root()->render(outputfile);
    }
    GPPtr<GPFunctionDataBase> base = new GPFunctionDataBase();
    {
        GPPtr<GPStreamWrap> input = GPStreamFactory::NewStream("func.xml");
        base->loadXml(input.get());
    }
    {
        GPPtr<GPFunctionFrontEndProducer> producer = new GPFunctionFrontEndProducer(base.get());
        GPPtr<GPFunctionTree> tree = producer->vCreateFromFormula(gFormulaNew, std::vector<const IStatusType*>());
        FUNC_PRINT_ALL(tree->dump().c_str(), s);
    }
    return 1;
}
