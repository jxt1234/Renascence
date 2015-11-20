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

static const char* gFormula = "TrPackageSaturation(TrPackageFilterTransformFromRegress(TrPackageCompse(TrPackageCompse(x0,TrPackageSaturation(x1)),TrPackageSaturation(x3)), TrPackageFilterMatrixRegress(x2, TrPackageCompse(TrPackageCompse(x0,TrPackageSaturation(x1)),TrPackageSaturation(x3)))))";

static int _test_main()
{
    GPFunctionDataBase* base = GPFactory::createDataBase("func.xml", NULL);
    AUTOCLEAN(base);
    {
        GPProducer* sys = GPFactory::createProducer(base, GPFactory::STREAM);
        AUTOCLEAN(sys);
        GPPtr<GPFunctionTree> tree = sys->getFront()->vCreateFromFormula(gFormula);
        GPPtr<GPMultiLayerTree> multiTree = new GPMultiLayerTree(tree.get());
        auto layers = multiTree->layers();
        for (size_t i=0; i<layers.size(); ++i)
        {
            GPPRINT("The %ld Layer: \n", i);
            for (auto iter : layers[i])
            {
                std::ostringstream output;
                iter.second->render(output);
                GPPRINT("%d:%s\n", iter.first, output.str().c_str());
            }
            GPPRINT("\n");
        }
    }
    return 1;
}
void test_main()
{
    GPPtr<GPFunctionDataBase> base = GPFactory::createDataBase("func.xml", NULL);
    {
        GPPtr<GPProducer> gen = GPFactory::createProducer(base.get(), GPFactory::STREAM);
        xmlReader r;
        auto n = r.loadFile("stream_test.xml");
        GPPtr<IGPAutoDefFunction> gp = gen->createFunction(n);
        GPPtr<GPTreeNode> node = gp->vSave();
        GPPtr<GPWStreamWrap> outp = GPStreamFactory::NewWStream("output/GPStreamTest.xml");
        xmlReader::dumpNodes(node.get(), outp.get());
        auto bmptype = base->vQueryType("TrBmp");
        outp->flush();
        {
            GPContents contents;
            GPPtr<GPStreamWrap> inputjpeg = GPStreamFactory::NewStream("input.jpg", GPStreamFactory::FILE);
            auto pic = bmptype->vLoad(inputjpeg.get());
            contents.push(pic, bmptype);
            inputjpeg = GPStreamFactory::NewStream("output.jpg", GPStreamFactory::FILE);
            pic = bmptype->vLoad(inputjpeg.get());
            contents.push(pic, bmptype);
            inputjpeg = GPStreamFactory::NewStream("input_sharp.jpg", GPStreamFactory::FILE);
            pic = bmptype->vLoad(inputjpeg.get());
            contents.push(pic, bmptype);
            inputjpeg = GPStreamFactory::NewStream("output.jpg", GPStreamFactory::FILE);
            pic = bmptype->vLoad(inputjpeg.get());
            contents.push(pic, bmptype);
            
            GPContents* out = gp->vRun(&contents);
            GPASSERT(NULL!=out);
            GPASSERT(out->size() == 1);
            GPPtr<GPWStreamWrap> outputjpeg = GPStreamFactory::NewWStream("output/GPStreamTest.jpg", GPStreamFactory::FILE);
            out->contents[0].type->vSave(out->get(0), outputjpeg.get());
            out->clear();
            delete out;
            contents.clear();
        }
        gp = gen->createFunction(gFormula);
        {
            GPContents contents;
            GPPtr<GPStreamWrap> inputjpeg = GPStreamFactory::NewStream("input.jpg", GPStreamFactory::FILE);
            auto pic = bmptype->vLoad(inputjpeg.get());
            contents.push(pic, bmptype);
            inputjpeg = GPStreamFactory::NewStream("output.jpg", GPStreamFactory::FILE);
            pic = bmptype->vLoad(inputjpeg.get());
            contents.push(pic, bmptype);
            inputjpeg = GPStreamFactory::NewStream("input_sharp.jpg", GPStreamFactory::FILE);
            pic = bmptype->vLoad(inputjpeg.get());
            contents.push(pic, bmptype);
            inputjpeg = GPStreamFactory::NewStream("output.jpg", GPStreamFactory::FILE);
            pic = bmptype->vLoad(inputjpeg.get());
            contents.push(pic, bmptype);
            
            GPContents* out = gp->vRun(&contents);
            GPASSERT(NULL!=out);
            GPASSERT(out->size() == 1);
            GPPtr<GPWStreamWrap> outputjpeg = GPStreamFactory::NewWStream("output/GPStreamTest2.jpg", GPStreamFactory::FILE);
            out->contents[0].type->vSave(out->get(0), outputjpeg.get());
            out->clear();
            delete out;
            contents.clear();
        }
    }
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

