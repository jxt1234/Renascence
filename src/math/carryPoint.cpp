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
