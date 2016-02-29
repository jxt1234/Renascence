#include "frontend/GPFormulaTree.h"
#include <iostream>
#include <fstream>
#include "math/GPSingleTree.h"

static const char* gFormula = "TrPackageSaturation(TrPackageFilterTransformFromRegress(TrPackageCompse(TrPackageCompse(x0,TrPackageSaturation(x1)),TrPackageSaturation(x3)), TrPackageFilterMatrixRegress(x2, TrPackageCompse(TrPackageCompse(x0,TrPackageSaturation(x1)),ADF[Input, x5]))))";

static const char* gFormulaNew =
"REDUCE(MAP([SPLIT(A), SPLIT(B)], Product(x0, x1), [[a0, a1], [b0, b1]]->[a0,b1,a1], a1==b0), Add(x0, x1), [a0, a1, a2]->[a0, a1])";
int main()
{
    GPPtr<GPSingleTree> tree = GPSingleTree::createFromFormula("x1*y1+z2/y1*x1", "x1 y1 z2");
    GPFLOAT v[3];
    v[0] = 0.5;
    v[1] = 0.8;
    v[2] = 2;
    FUNC_PRINT_ALL(tree->compute(v), f);
    return 1;
}
