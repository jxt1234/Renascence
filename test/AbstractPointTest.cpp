#include "AbstractPointTest.h"
#include <iostream>

using namespace std;

int AbstractPointTest::gNum = 0;

void AbstractPointTest::loadChain(const vector<int>& chain)
{
    for (int i=0; i<chain.size(); ++i)
    {
        AbstractPointTest* child = new AbstractPointTest;
        addPoint(child);
        for (int j=0; j<chain[i]; ++j)
        {
            child->addPoint(new AbstractPointTest);
        }
        cout << gNum<<endl;
    }
}
