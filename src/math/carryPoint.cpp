#include "math/carryPoint.h"

bool carryPoint::grow()
{
    mGrow = true;
    while (!(this->vGrow()))
    {
        if (!(this->vNext()))
        {
            return false;
        }
    }
    for (int i=0; i<mChild.size(); ++i)
    {
        mChild[i]->grow();
    }
    return true;
}
bool carryPoint::next()
{
    if (!mGrow) return grow();
    //Self next
    if (mChild.empty())
    {
        return this->vNext();
    }
    //Child next
    for (int i=0; i<mChild.size(); ++i)
    {
        if (mChild[i]->next())
        {
            return true;
        }
    }
    //If every child is next ok, it should delete all children and self next
    clear();
    if (this->vNext())
    {
        mGrow = false;
        return this->next();
    }
    return false;
}

void carryPoint::clear()
{
    if (mChild.empty()) return;
    for (int i=0; i<mChild.size(); ++i)
    {
        delete mChild[i];
    }
    mChild.clear();
}
