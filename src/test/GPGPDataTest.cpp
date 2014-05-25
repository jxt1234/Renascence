#include "test/GPTest.h"
#include "system/xmlGenerateSystem.h"
#include "xml/xmlGPDataLoader.h"
#include <fstream>
using namespace std;

class GPGPDataTest:public GPTest
{
    public:
        virtual void run()
        {
            xmlGenerateSystem gen;
            gen.addXml("func.xml");
            xmlGPDataLoader* l=new xmlGPDataLoader(gen);
            l->loadFile("test/GPGPDataTest.xml");
            const GPData& data = l->get();
            ofstream of("output/GPGPDataTestResult.xml");
            data.print(of);
            of.close();
            delete l;
        }
        GPGPDataTest(){}
        virtual ~GPGPDataTest(){}
};

static GPTestRegister<GPGPDataTest> a("GPGPDataTest");
