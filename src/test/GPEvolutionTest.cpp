#include "test/GPTest.h"
#include "api/evolution.h"
#include "xml/xmlGPLoader.h"
class GPEvolutionTest:public GPTest
{
    public:
        virtual void run()
        {
            status_init();
            GP_evolution(10, 2, "func.xml", NULL, "result.xml");
        }
        GPEvolutionTest(){}
        virtual ~GPEvolutionTest(){}
};

static GPTestRegister<GPEvolutionTest> a("RunEvolutionTest");
