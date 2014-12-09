#include "test/GPTest.h"
#include "xml/xmlTree.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
class GPxmlTreeTest:public GPTest
{
    public:
        virtual void run()
        {
            xmlTree tree;
            tree.loadFile("output/result.xml");
            ofstream file("output/result_test.xml");
            tree.print(file);
        }
        GPxmlTreeTest(){}
        virtual ~GPxmlTreeTest(){}
};

static GPTestRegister<GPxmlTreeTest> a("GPxmlTreeTest");
