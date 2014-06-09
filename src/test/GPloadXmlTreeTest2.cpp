#include "test/GPTest.h"
#include "evolution/evolutionTree.h"
#include "system/xmlGenerateSystem.h"

#include <fstream>

using namespace std;

class GPloadXmlTreeTest2:public GPTest
{
    public:
        virtual void run()
        {
            xmlGenerateSystem gen;
            gen.addXml("func.xml", NULL, NULL);
            ifstream f("result.xml");
            IGPAutoDefFunction* gp = gen.vCreateFunctionFromIS(f);
            f.close();
            ofstream file;
            file.open("output/GPloadXmlTreeTest2.xml");
            gp->save(file);
            file.close();
            gp->decRef();
        }
        GPloadXmlTreeTest2(){}
        virtual ~GPloadXmlTreeTest2(){}
};

static GPTestRegister<GPloadXmlTreeTest2> a("GPloadXmlTreeTest2");
