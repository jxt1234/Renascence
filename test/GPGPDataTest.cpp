#include "test/GPTest.h"
#include <fstream>
#include "core/GPFunctionDataBase.h"
#include "core/GPFactory.h"
using namespace std;
//TODO
#if 0
class GPGPDataTest:public GPTest
{
    public:
        virtual void run()
        {
            GPFunctionDataBase* base = GPFactory::createDataBase("func.xml", NULL);
            AUTOCLEAN(base);
            xmlReader r;
            r.loadFile("test/GPGPDataTest.xml");
            xmlGPDataLoader* l=new xmlGPDataLoader(*base);
            l->loadFile("test/GPGPDataTest.xml");
            GPData* data = l->get();
            ofstream of("output/GPGPDataTestResult.xml");
            data->print(of);
            of.close();
            delete l;
        }
        GPGPDataTest(){}
        virtual ~GPGPDataTest(){}
};

static GPTestRegister<GPGPDataTest> a("GPGPDataTest");
#endif