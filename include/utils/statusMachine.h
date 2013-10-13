#ifndef UTILS_STATUS_MACHINE_H
#define UTILS_STATUS_MACHINE_H
#include <vector>

class statusMachine
{
    typedef int (*statusFunc)(statusMachine*);
    public:
        void run();
        statusMachine(){}
        virtual ~statusMachine(){}
    private:
        int mStatus;
        std::vector<statusFunc> mFuncs;
};
#endif
