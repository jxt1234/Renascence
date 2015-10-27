#include "AbstractPointTest.h"
#include <iostream>

using namespace std;

int GPAbstractPointTest::gNum = 0;

void GPAbstractPointTest::loadChain(const vector<int>& chain)
{
    for (int i=0; i<chain.size(); ++i)
    {
        GPAbstractPointTest* child = new GPAbstractPointTest;
        addPoint(child);
        for (int j=0; j<chain[i]; ++j)
        {
            child->addPoint(new GPAbstractPointTest);
        }
        cout << gNum<<endl;
    }
}
