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
#include "core/GPStreamFactory.h"
using namespace std;
static int test_main()
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
        outp->flush();
        {
            GPContents contents;
            auto bmptype = base->vQueryType("TrBmp");
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

