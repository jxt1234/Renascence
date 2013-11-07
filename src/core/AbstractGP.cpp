#include "core/AbstractGP.h"

AbstractGP::AbstractGP(const AbstractGP& gp)
{
}

void AbstractGP::operator=(const AbstractGP& gp)
{
    for (int i=0; i<mChildren.size(); ++i)
    {
        delete mChildren[i];
    }
    mFunc = gp.mFunc;
    mStatus = gp.mStatus;
    for (int i=0; i<gp.mChildren.size(); ++i)
    {
        addPoint(*(gp.mChildren[i]));
    }
}
AbstractGP::~AbstractGP()
{
    if (NULL!=mSave)
    {
        delete mSave;
    }
}
