#include <fstream>
#include <iostream>
#include "test/GPTest.h"
#include "core/GPFunctionDataBase.h"
#include "core/GPFactory.h"
#include "core/GPProducer.h"
#include "producer/GPGraphicADF.h"
using namespace std;
class GPGraphicTest:public GPTest
{
    public:
        virtual void run()
        {
            GPPtr<GPFunctionDataBase> base = GPFactory::createDataBase("func.xml", NULL);
            {
                GPPtr<GPProducer> gen = GPFactory::createProducer(base.get(), GPFactory::GRAPHIC);
                ifstream f("graphic_test.xml");
                GPPtr<IGPAutoDefFunction> gp = gen->vCreateFunctionFromIS(f);
                f.close();
                ofstream out("output/GPGraphicTest.xml");
                gp->save(out);
                out.close();
                {
                    GPContents contents;
                    contents.push(NULL, NULL);
                    GPContents* out = gp->run(&contents);
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
