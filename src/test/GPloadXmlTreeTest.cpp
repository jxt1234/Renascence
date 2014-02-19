#include "test/GPTest.h"
#include "evolution/evolutionTree.h"
#include "system/xmlGenerateSystem.h"

#include <fstream>

using namespace std;

class GPloadXmlTreeTest:public GPTest
{
    public:
        virtual void run()
        {
            xmlGenerateSystem gen;
            gen.addXml("func.xml", NULL, false);
            evolutionTree::setGenSystem(&gen);
            xmlTree _tree;
            _tree.loadFile("result.xml");
            mutateTree* tree = mutateTree::loadXmlTree(&_tree, &gen);
            string result = gen.xmlPrint(tree);
            ofstream file;
            file.open("output/result_test2.xml");
            file<<result;
            file.close();
            delete tree;
        }
        GPloadXmlTreeTest(){}
        virtual ~GPloadXmlTreeTest(){}
};

static GPTestRegister<GPloadXmlTreeTest> a("GPloadXmlTreeTest");
