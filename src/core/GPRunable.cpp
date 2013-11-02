/******************************************************************
   Copyright 2013, Jiang Xiao-tang

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
******************************************************************/
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





