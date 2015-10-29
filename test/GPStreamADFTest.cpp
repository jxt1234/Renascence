#include "test/GPTest.h"
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
#include "backend/GPGraphicADF.h"
#include "core/GPStreamFactory.h"
using namespace std;
class GPStreamADFTest:public GPTest
{
    public:
        virtual void run();
        GPStreamADFTest(){}
        virtual ~GPStreamADFTest(){}
};
void GPStreamADFTest::run()
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
        gp = gen->createFunction("TrPackageSaturation(TrPackageFilterTransformFromRegress(TrPackageCompse(x0,x1), TrPackageFilterMatrixRegress(x2, x3)))");
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

static GPTestRegister<GPStreamADFTest> a("GPStreamADFTest");
