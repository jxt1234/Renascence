#include "frontend/GPFormulaTree.h"
#include <iostream>
#include <fstream>
#include "math/GPSingleTree.h"

static const char* gFormulas[] = {
    "x*y+z",
    "ln((x*z+y))",
    "cos(sin(x+y)*z)",
    "x==y&&y==z"
};
int main()
{
    GPFLOAT v[3];
    v[0] = 0.5;
    v[1] = 0.8;
    v[2] = 1;
    for (int i=0; i<sizeof(gFormulas)/sizeof(const char*); ++i)
    {
        GPPtr<GPSingleTree> tree = GPSingleTree::createFromFormula(gFormulas[i], "x y z");
        FUNC_PRINT_ALL(tree->compute(v), f);
    }
    return 1;
}
