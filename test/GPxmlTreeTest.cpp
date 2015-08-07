#include "test/GPTest.h"
#include "xml/xmlTree.h"
#include "xml/xmlReader.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
class GPxmlTreeTest:public GPTest
{
    public:
        virtual void run()
        {
            xmlReader r;
            auto n = r.loadFile("result.xml");
            xmlTree tree(n);
            ofstream file("output/result_test.xml");
            tree.print(file);
        }
        GPxmlTreeTest(){}
        virtual ~GPxmlTreeTest(){}
};

static GPTestRegister<GPxmlTreeTest> a("GPxmlTreeTest");
