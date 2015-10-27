#include "math/GPAbstractPoint.h"

class GPAbstractPointTest:public GPAbstractPoint
{
    public:
        GPAbstractPointTest(){gNum++;}
        virtual ~GPAbstractPointTest(){gNum--;}
        void loadChain(const std::vector<int>& chain);
        static int number(){return gNum;}
    protected:
        static int gNum;
};
