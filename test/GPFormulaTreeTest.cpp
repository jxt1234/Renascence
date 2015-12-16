#include "test/GPTest.h"
#include "frontend/GPFormulaTree.h"
#include <iostream>
#include <fstream>
class GPFormulaTreeTest:public GPTest
{
    public:
        virtual void run();
        GPFormulaTreeTest(){}
        virtual ~GPFormulaTreeTest(){}
};
static const char* gFormula = "TrPackageSaturation(TrPackageFilterTransformFromRegress(TrPackageCompse(TrPackageCompse(x0,TrPackageSaturation(x1)),TrPackageSaturation(x3)), TrPackageFilterMatrixRegress(x2, TrPackageCompse(TrPackageCompse(x0,TrPackageSaturation(x1)),ADF[Input, x5]))))";
void GPFormulaTreeTest::run()
{
    GPFormulaTree tree;
    tree.setFormula(gFormula);
    std::ofstream output("output/GPFormulaTreeTest.txt");
    tree.root()->render(output);
}

static GPTestRegister<GPFormulaTreeTest> a("GPFormulaTreeTest");
