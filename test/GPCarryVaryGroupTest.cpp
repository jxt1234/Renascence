#include "test/GPTest.h"
#include "math/GPCarryVaryGroup.h"
#include <fstream>

class GPCarryVaryGroupTest:public GPTest
{
    public:
        virtual void run();
        GPCarryVaryGroupTest(){}
        virtual ~GPCarryVaryGroupTest(){}
};
void GPCarryVaryGroupTest::run()
{
    unsigned int dims[] = {4, 5, 3};
    unsigned int pos[3];
    GPCarryVaryGroup group(dims, 3);
    std::ofstream output("output/GPCarryVaryGroupTest.txt");
    group.start(pos, 3);
    do
    {
        output << pos[0] << ", "<<pos[1] << ", " << pos[2] << "\n";
    } while (group.next(pos, 3));
}

static GPTestRegister<GPCarryVaryGroupTest> a("GPCarryVaryGroupTest");
