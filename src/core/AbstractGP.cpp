#include "core/AbstractGP.h"

using namespace std;

AbstractGP::AbstractGP(const AbstractGP& gp)
{
    mFunc = gp.mFunc;
    mStatus = gp.mStatus;
    AbstractGPCopy c;
    for (int i=0; i<gp.mChildren.size(); ++i)
    {
        addPoint(deepCopy((gp.mChildren[i]), &c));
    }
}

void AbstractGP::operator=(const AbstractGP& gp)
{
    for (int i=0; i<mChildren.size(); ++i)
    {
        delete mChildren[i];
    }
    mFunc = gp.mFunc;
    mStatus = gp.mStatus;
    AbstractGPCopy c;
    for (int i=0; i<gp.mChildren.size(); ++i)
    {
        addPoint(deepCopy((gp.mChildren[i]), &c));
    }
}
AbstractGP::~AbstractGP()
{
    if (NULL!=mSave)
    {
        delete mSave;
    }
}

GP_Output AbstractGP::up_compute(IFunctionDataBase* map)
{
    GP_Output result;
    if (NULL!=mSave)
    {
        result = *mSave;
        for (int i=0; i<result.output.size(); ++i)
        {
            result.output[i].freeCallBack = NULL;
        }
        return result;
    }
    computeFunction comp = map->vGetCompute(mFunc);
    vector<GP_Output::GP_Unit> inputMap;
    vector<void*> children;
    //Get Inputs from childern point
    for (int i=0; i<mChildren.size(); ++i)
    {
        AbstractGP* p = dynamic_cast<AbstractGP*>(mChildren[i]);
        GP_Output out = p->up_compute(map);
        vector<GP_Output::GP_Unit>& output_unit = out.output;
        for (int j=0; j<output_unit.size(); ++j)
        {
            inputMap.push_back(output_unit[j]);
            children.push_back(output_unit[j].content);
        }
    }
    vector<void*> constValue;
    if (-1 != mStatus)
    {
        constValue = status_queryContent(mStatus);
    }
    {
#ifdef DEBUG_TIMES
        GP_Clock c(mFunc);
#endif
        result = comp(children, constValue);
    }
    //Free All children' memory
    for (int i=0; i < inputMap.size(); ++i)
    {
        GP_Output::GP_Unit& out = inputMap[i];
        if (NULL!=out.freeCallBack)
        {
            out.freeCallBack(out.content);
        }
    }
    return result;
}
