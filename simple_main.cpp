#include "frontend/GPFormulaTree.h"
#include <iostream>
#include <fstream>

static const char* gFormula = "TrPackageSaturation(TrPackageFilterTransformFromRegress(TrPackageCompse(TrPackageCompse(x0,TrPackageSaturation(x1)),TrPackageSaturation(x3)), TrPackageFilterMatrixRegress(x2, TrPackageCompse(TrPackageCompse(x0,TrPackageSaturation(x1)),ADF[Input, x5]))))";

static const char* gFormulaNew =
"REDUCE(MAP([SPLIT(A), SPLIT(B)], Product(x0, x1), [[a0, a1], [b0, b1]]->[a0,b1,a1], a1==b0), Add(x0, x1), [a0, a1, a2]->[a0, a1])";
int main()
{
    std::ofstream outputfile("/Users/jiangxiaotang/Documents/Genetic-Program-Frame/FormulaTreeTest.txt");
    GPFormulaTree tree;
    tree.setFormula(gFormulaNew);
    tree.root()->render(outputfile);
    return 1;
}
