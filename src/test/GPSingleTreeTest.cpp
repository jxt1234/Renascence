#include "test/GPTest.h"
#include "math/GPSingleTree.h"
using namespace std;

class GPSingleTreeTest:public GPTest
{
    public:
        virtual void run()
        {
        }
        GPSingleTreeTest(){}
        virtual ~GPSingleTreeTest(){}
};

static GPTestRegister<GPSingleTreeTest> a("GPSingleTreeTest");
