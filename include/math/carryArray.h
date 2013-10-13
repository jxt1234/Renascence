#ifndef MATH_CARRY_ARRAY_H
#define MATH_CARRY_ARRAY_H
#include <vector>
using namespace std;

class carryArray
{
    public:
        carryArray(int size=0, int maxNumber=0);
        ~carryArray(){}
        carryArray& operator++();
        void reset();
        bool reachEnd();
        const vector<int>& getCurrent(){return mQueue;}
    protected:
        void runCarry();
    private:
        vector<int> mQueue;
        int mCarryNumber;
};



#endif
