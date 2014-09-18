/******************************************************************
   Copyright 2014, Jiang Xiao-tang

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
#include "GPSearchTree.h"
#include <algorithm>
using namespace std;

GPSearchTreePoint::GPSearchTreePoint(const GPFunctionDataBase* base, FUNC* f, GPSearchTreePoint* depend, const vector<const IStatusType*>& permitInput):mInputs(permitInput)
{
    mBase = base;
    mF = f;
    mDepend = depend;
    FUNCTEAM forbid = _getDependFunction();
    mValid = true;
    mCur = 0;
    mUseCarry = true;
    /*If fixTable is not empty, use fixTable, else generate group by inputType*/
    if (!(f->fixTable).empty())
    {
        mUseCarry = false;
        //TODO Turn fixTable be GROUP type, such avoid this convert
        const vector<vector<int> >& intTable = f->fixTable;
        GROUP fixTable;
        for (int i=0; i<intTable.size(); ++i)
        {
            FUNCTEAM team;
            for (int j=0; j<intTable[i].size(); ++j)
            {
                team.push_back(mBase->vQueryFunctionById(intTable[i][j]));
            }
            fixTable.push_back(team);
        }
        _addTeam(fixTable, forbid, mGroup.mBase);
        if (fixTable.size()>0 && mGroup.mBase.empty())
        {
            mValid = false;
        }
    }
    else
    {
        //TODO Support multi output
        const vector<const IStatusType*> input = f->inputType;
        for (int i=0; i<input.size(); ++i)
        {
            FUNCTEAM team = mBase->vSearchFunction(input[i]);
            FUNCTEAM::iterator iter = team.begin();
            for (;iter!=team.end();iter++)
            {
                if (find(forbid.begin(), forbid.end(), *iter)!=forbid.end())
                {
                    iter = team.erase(iter);
                }
            }
            if (find(permitInput.begin(), permitInput.end(), input[i])!=permitInput.end())
            {
                team.insert(team.begin(), NULL);
            }
            if (team.empty())
            {
                //Make this point invalid
                mValid = false;
                break;
            }
            mGroup.mBase.push_back(team);
        }
        mGroup.reset();
    }
}

GPSearchTreePoint::~GPSearchTreePoint()
{
}

void GPSearchTreePoint::_addTeam(const GROUP& origin, const FUNCTEAM& forbid, GROUP& output)
{
    for (int i=0; i<origin.size(); ++i)
    {
        const FUNCTEAM& team = origin[i];
        bool valid = true;
        for (int j=0; j<team.size(); ++j)
        {
            FUNC* _f = (team[j]);
            for (int k=0; k<forbid.size(); ++k)
            {
                if (_f == forbid[k])
                {
                    valid = false;
                    break;
                }
            }
            if (!valid)
            {
                break;
            }
        }
        if (valid)
        {
            output.push_back(team);
        }
    }
}


bool GPSearchTreePoint::vGrow()
{
    const FUNCTEAM& team = current();
    for (int i=0; i<team.size(); ++i)
    {
        if (NULL == team[i])
        {
            continue;
        }
        GPSearchTreePoint* p = new GPSearchTreePoint(mBase, team[i], this, mInputs);
        mChild.push_back(p);
        if (p->invalid())
        {
            return false;
        }
    }
    return true;
}

bool GPSearchTreePoint::invalid() const
{
    return !mValid;
}

const GPSearchTreePoint::FUNCTEAM& GPSearchTreePoint::current() const
{
    if (mUseCarry)
    {
        return mGroup.current();
    }
    else
    {
        return mGroup.mBase[mCur];
    }
}

GPTreeADFPoint* GPSearchTreePoint::output() const
{
    GPTreeADFPoint* res = new GPTreeADFPoint(mF);
    /*Get children from child search point, recurse*/
    for (int i=0; i<mChild.size(); ++i)
    {
        GPSearchTreePoint* p = (GPSearchTreePoint*)mChild[i];
        GPTreeADFPoint* child = p->output();
        res->addPoint(child);
    }
    return res;
}

GPSearchTreePoint::FUNCTEAM GPSearchTreePoint::_getDependFunction()
{
    FUNCTEAM res;
    GPSearchTreePoint* p = this;
    while(NULL != p)
    {
        res.push_back(p->mF);
        p = (GPSearchTreePoint*)(p->mDepend);
    }
    return res;
}

bool GPSearchTreePoint::vNext()
{
    if (mUseCarry)
    {
        return mGroup.next();
    }
    else
    {
        int cur = mCur+1;
        if (cur >= mGroup.mBase.size())
        {
            return false;
        }
        else
        {
            mCur = cur;
            return true;
        }
    }
}
