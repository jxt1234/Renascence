#include "frontend/GPFormulaTree.h"
#include <iostream>
#include <fstream>

static const char* gFormula = "TrPackageSaturation(TrPackageFilterTransformFromRegress(TrPackageCompse(TrPackageCompse(x0,TrPackageSaturation(x1)),TrPackageSaturation(x3)), TrPackageFilterMatrixRegress(x2, TrPackageCompse(TrPackageCompse(x0,TrPackageSaturation(x1)),ADF[Input, x5]))))";

static const char* gFormulaNew =
"MERGE(REDUCE(INPUT(MAP(INPUT(A, B), Product(x0, x1), FOR(KEY(a0, a1), KEY(b0, b1)), TO(KEY(a0,b1,a1)), CONDITION(a1==b0))), Add(x0, x1), FOR(KEY(a0, a1, a2)), TO(KEY(a0, a1))))";
int main()
{
    std::ofstream outputfile("/Users/jiangxiaotang/Documents/Genetic-Program-Frame/.test.xml");
    GPFormulaTree tree;
    tree.setFormula(gFormulaNew);
    tree.root()->render(outputfile);
    return 1;
}
