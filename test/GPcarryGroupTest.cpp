#include "test/GPTest.h"
#include "math/carryGroup.h"
#include <fstream>
using namespace std;
class GPcarryGroupTest:public GPTest
{
    public:
        virtual void run();
        GPcarryGroupTest(){}
        virtual ~GPcarryGroupTest(){}
};
void GPcarryGroupTest::run()
{
    carryGroup<int> group;
    ofstream out("output/GPcarryGroupTest.txt");
    out << "List\n";
    for (int i=0; i<4; ++i)
    {
        vector<int> iters;
        for(int j=0; j<i+1; ++j)
        {
            iters.push_back(j*j+i);
        }
        for (int j=0; j<iters.size(); ++j)
        {
            out <<iters[j] << " ";
        }
        out << "\n";
        group.mBase.push_back(iters);
    }
    group.reset();
    out << "Iter result\n";
    do
    {
        vector<int> iters = group.current();
        for (int j=0; j<iters.size(); ++j)
        {
            out <<iters[j] << " ";
        }
        out << "\n";
    } while(group.next());
}

static GPTestRegister<GPcarryGroupTest> a("GPcarryGroupTest");
