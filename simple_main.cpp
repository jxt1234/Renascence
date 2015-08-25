#include "user/GPAPI.h"
#include <assert.h>
#include <fstream>
#include <iostream>
#include <string>
#include <fstream>
#include <iostream>
#include "core/GPFunctionDataBase.h"
#include "core/GPFactory.h"
#include "core/GPStreamFactory.h"
#include "core/GPProducer.h"
#include "producer/GPGraphicADF.h"
/*TODO*/
#include "core/GPStreamFactory.h"
using namespace std;
static int test_main()
{
    GPPtr<GPFunctionDataBase> base = GPFactory::createDataBase("func.xml", NULL);
    {
        GPPtr<GPStreamWrap> inputjpeg = GPStreamFactory::NewStream("input.jpg", GPStreamFactory::FILE);
        auto bmptype = base->vQueryType("TrBmp");
        auto pic = bmptype->vLoad(inputjpeg.get());
        GPPtr<GPWStreamWrap> outputjpeg = GPStreamFactory::NewWStream("input_test_simple.jpg", GPStreamFactory::FILE);
        bmptype->vSave(pic, outputjpeg.get());
        bmptype->vFree(pic);
        GPPtr<GPProducer> gen = GPFactory::createProducer(base.get(), GPFactory::GRAPHIC);
        xmlReader r;
        auto n = r.loadFile("graphic_test.xml");
        GPPtr<IGPAutoDefFunction> gp = gen->vCreateFunctionFromNode(n);
        GPPtr<GPTreeNode> node = gp->vSave();
        GPPtr<GPWStreamWrap> outp = GPStreamFactory::NewWStream("output/GPGraphicTest.xml");
        xmlReader::dumpNodes(node.get(), outp.get());
        {
            GPContents contents;
            contents.push(NULL, NULL);
            GPContents* out = gp->vRun(&contents);
            GPASSERT(out->size() == 1);
            double* result = (double*)out->get(0);
            cout << *result <<endl;
            out->clear();
            delete out;
        }
    }
    return 1;
}
int main()
{
    GP_Set_Lib_Path("/Users/jiangxiaotang/Documents/Genetic-Program-Frame/");
    GP_Set_Stream_Path("/Users/jiangxiaotang/Documents/Genetic-Program-Frame/");
    return test_main();
}

