#include "test/GPTest.h"

#include "core/GPFunctionDataBase.h"
#include "core/GPProducer.h"
#include "core/GPFactory.h"
#include <fstream>
#include <string>
#include <iostream>
#include <core/GPStreamFactory.h>
#include "head.h"

using namespace std;

class GPloadXmlTreeTest2:public GPTest
{
public:
    virtual void run()
    {
        GPFunctionDataBase* base = GPFactory::createDataBase("func.xml", NULL);
        AUTOCLEAN(base);
        {
            GPProducer* gen = GPFactory::createProducer(base);
            AUTOCLEAN(gen);
            {
                xmlReader r;
                auto f = r.loadFile("tree_result.xml");
                IGPAutoDefFunction* gp = gen->createFunction(f);
                AUTOCLEAN(gp);
                GPPtr<GPWStreamWrap> file = GPStreamFactory::NewWStream("output/GPloadXmlTreeTest.xml", GPStreamFactory::FILE);
                GPPtr<GPTreeNode> dump = gp->vSave();
                xmlReader::dumpNodes(dump.get(), file.get());
                IGPAutoDefFunction* gp2 = gp->vCopy();
                AUTOCLEAN(gp2);
                file = GPStreamFactory::NewWStream("output/GPloadXmlTreeTest_copy.xml", GPStreamFactory::FILE);
                dump = gp2->vSave();
                xmlReader::dumpNodes(dump.get(), file.get());
                file = GPStreamFactory::NewWStream("output/GPloadXmlTreeTest_mutate.xml", GPStreamFactory::FILE);
                {
                    bool change;
                    int n = gp2->vMapStructure(NULL, change);
                    GPPtr<GPParameter> p = new GPParameter(n);
                    p->clear(0.6f);
                    gp2->vMapStructure(p.get(), change);
                }
                dump = gp2->vSave();
                xmlReader::dumpNodes(dump.get(), file.get());
            }
        }
    }
    GPloadXmlTreeTest2(){}
    virtual ~GPloadXmlTreeTest2(){}
};

static GPTestRegister<GPloadXmlTreeTest2> a("GPloadXmlTreeTest");
