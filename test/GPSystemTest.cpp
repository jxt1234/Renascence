#include "test/GPTest.h"
#include "platform/GPSystem.h"
#include <iostream>
class GPSystemTest:public GPTest
{
    public:
        virtual void run();
        GPSystemTest(){}
        virtual ~GPSystemTest(){}
};
void GPSystemTest::run()
{
    auto result = system_list_dir_files("./output");
    for (auto s : result)
    {
        std::cout << s << std::endl;
    }
}

static GPTestRegister<GPSystemTest> a("GPSystemTest");
