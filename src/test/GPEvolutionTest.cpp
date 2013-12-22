#include "test/GPTest.h"
#include "api/evolution.h"
#include "xml/xmlGPLoader.h"
class GPEvolutionTest:public GPTest
{
    public:
        virtual void run()
        {
            GP_evolution(10, 10, "func.xml", NULL, "result.xml");
        }
        GPEvolutionTest(){}
        virtual ~GPEvolutionTest(){}
};

static GPTestRegister<GPEvolutionTest> a("RunEvolutionTest");
