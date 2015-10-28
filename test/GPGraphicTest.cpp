#include <fstream>
#include <iostream>
#include "test/GPTest.h"
#include "core/GPFunctionDataBase.h"
#include "core/GPFactory.h"
#include "core/GPStreamFactory.h"
#include "core/GPProducer.h"
#include "backend/GPGraphicADF.h"
using namespace std;
class GPGraphicTest:public GPTest
{
    public:
        virtual void run()
        {
            GPPtr<GPFunctionDataBase> base = GPFactory::createDataBase("func.xml", NULL);
            {
                GPPtr<GPProducer> gen = GPFactory::createProducer(base.get(), GPFactory::GRAPHIC);
                xmlReader r;
                auto n = r.loadFile("graphic_test.xml");
                GPPtr<IGPAutoDefFunction> gp = gen->createFunction(n);
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
            GPASSERT(0 == GPGraphicADF::Point::numberOfInstance());
        }
        GPGraphicTest(){}
        virtual ~GPGraphicTest(){}
};

static GPTestRegister<GPGraphicTest> a("GPGraphicTest");
