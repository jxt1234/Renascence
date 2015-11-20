#include "user/GPAPI.h"
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
        GPPtr<GPFunctionTree> tree = sys->getFront()->vCreateFromFormula("TrPackageSaturation(TrPackageFilterTransformFromRegress(TrPackageCompse(TrPackageCompse(x0,TrPackageSaturation(x1)),TrPackageSaturation(x1)), TrPackageFilterMatrixRegress(x2, TrPackageCompse(TrPackageCompse(x0,TrPackageSaturation(x1)),TrPackageSaturation(x1)))))");
        GPPtr<GPMultiLayerTree> multiTree = new GPMultiLayerTree(tree.get());
        auto layers = multiTree->layers();
        for (size_t i=0; i<layers.size(); ++i)
        {
            GPPRINT("The %ld Layer: \n", i);
            for (auto iter : layers[i])
            {
                GPPRINT("%d:%s, depth=%ld\n", iter.first, iter.second->function()->name.c_str(), iter.second->depth());
            }
            GPPRINT("\n");
        }
    }
    return 1;
}

int main()
{
    GP_Set_Lib_Path("/Users/jiangxiaotang/Documents/Genetic-Program-Frame/");
    GP_Set_Stream_Path("/Users/jiangxiaotang/Documents/Genetic-Program-Frame/");
    {
        test_main();
    }
    return 0;
}

