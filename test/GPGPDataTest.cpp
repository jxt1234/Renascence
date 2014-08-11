#include "test/GPTest.h"
#include <fstream>
#include "core/GPFunctionDataBase.h"
#include "core/GPFactory.h"
#include "core/xmlGPDataLoader.h"
using namespace std;

class GPGPDataTest:public GPTest
{
    public:
        virtual void run()
        {
            GPFunctionDataBase* base = GPFactory::createDataBase("func.xml", NULL);
            AUTOCLEAN(base);
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
