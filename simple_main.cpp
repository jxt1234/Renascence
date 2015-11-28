#include "user/GPAPI.h"
#include "core/GPProducer.h"
#include "frontend/GPFormulaTree.h"
#include <iostream>
#include "core/GPFactory.h"
#include "core/GPFunctionDataBase.h"
#include "head.h"
#include <fstream>
#include <iostream>
#include "core/GPFunctionDataBase.h"
#include "core/GPFactory.h"
#include "core/GPStreamFactory.h"
#include "core/GPProducer.h"
#include "frontend/GPFrontEndProducer.h"
#include "backend/GPGraphicADF.h"
#include "midend/GPMultiLayerTree.h"
#include <sstream>
using namespace std;

static int test_main()
{
    GPFunctionDataBase* base = GPFactory::createDataBase("func.xml", NULL);
    AUTOCLEAN(base);
    {
        GPProducer* sys = GPFactory::createProducer(base);
        AUTOCLEAN(sys);
        GPPtr<GPFunctionTree> tree = sys->getFront()->vCreateFromFormula("TrPackageSaturation(TrPackageFilterTransformFromRegress(TrPackageCompse(TrPackageCompse(x0,TrPackageSaturation(x1)),TrPackageSaturation(x1)), TrPackageFilterMatrixRegress(x2, TrPackageCompse(TrPackageCompse(x0,TrPackageSaturation(x1)),TrPackageSaturation(x1)))))");
        GPPtr<GPMultiLayerTree> multiTree = new GPMultiLayerTree(tree.get());
        auto layers = multiTree->layers();
        for (size_t i=0; i<layers.size(); ++i)
        {
            GPPRINT("The %ld Layer: ", i);
            for (auto iter : layers[i])
            {
                std::ostringstream formulas;
                iter.second->render(formulas);
                GPPRINT("%d:%s, ", iter.first, formulas.str().c_str());
            }
            GPPRINT_FL("\n");
        }
    }
    return 1;
}


int main()
{
    GP_Set_Stream_Path("/Users/jiangxiaotang/Documents/Genetic-Program-Frame/");
    GP_Set_Lib_Path("/Users/jiangxiaotang/Documents/Genetic-Program-Frame/");
    return test_main();
}