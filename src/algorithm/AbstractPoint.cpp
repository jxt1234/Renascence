#include "algorithm/AbstractPoint.h"
#include <stdlib.h>
#include <assert.h>
using namespace std;

AbstractPoint::~AbstractPoint()
{
    for (int i=0; i<mChildren.size(); ++i)
    {
        delete mChildren[i];
    }
}

void AbstractPoint::addPoint(AbstractPoint* p)
{
    assert(NULL!=p);
    mChildren.push_back(p);
}

AbstractPoint* AbstractPoint::deepCopy(AbstractPoint* src, AbstractPoint::IPointCopy* copy)
{
    assert(NULL!=src);
    assert(NULL!=copy);
    AbstractPoint* result;
    result = copy->copy(src);
    for (int i=0; i<(src->mChildren).size(); ++i)
    {
        AbstractPoint* child = deepCopy(src->mChildren[i], copy);
        result->addPoint(child);
    }
    return result;
}


