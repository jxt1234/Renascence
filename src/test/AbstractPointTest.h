#include "math/AbstractPoint.h"

class AbstractPointTest:public AbstractPoint
{
    public:
        AbstractPointTest(){gNum++;}
        virtual ~AbstractPointTest(){gNum--;}
        void loadChain(const std::vector<int>& chain);
        static int number(){return gNum;}
    protected:
        static int gNum;
};
