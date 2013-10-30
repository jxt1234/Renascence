#include "core/GPRunable.h"

#include <assert.h>

using namespace std;

IFunctionDataBase* GPRunable::mData = NULL;

void GPRunable::assignInputs(const vector<void*>& inputs)
{
    assert(inputs.size()>0);
    assert(inputs.size() == mSavePoints.size());
    for (int i=0; i<inputs.size(); ++i)
    {
        assert(NULL!=mSavePoints[i]);
        GP_Output* save = mSavePoints[i]->save;
        assert(NULL!=save);
        assert(save->output.size()==1);
        /*We needn't free inputs because it's managed outside*/
        save->output[0].content = inputs[i];
        save->output[0].freeCallBack = NULL;
    }
}

void GPRunable::refreshSavePoint()
{
    mSavePoints.clear();
    vector<GeneticPoint*> search = searchAllPoints();
    for (int i=0; i<search.size(); ++i)
    {
        if (search[i]->functionId < 0)
        {
            mSavePoints.push_back(search[i]);
        }
    }
}

GP_Output GPRunable::inputCompute(std::vector<void*> inputs, std::vector<void*> constValue)
{
    assert(NULL!=mData);
    refreshSavePoint();
    assignInputs(inputs);
    compute(mData);
}





