/******************************************************************
 Copyright 2016, Jiang Xiao-tang
 
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

#include "core/GPMultiTable.h"
#include "utils/GPDebug.h"
void* GPMultiTable::vGetFunction(const std::string& name)
{
    for (auto t:mTables)
    {
        void* f = t->vGetFunction(name);
        if (NULL!=f)
        {
            return f;
        }
    }
    return NULL;
}
GPMultiTable::GPMultiTable()
{
    
}
GPMultiTable::~GPMultiTable()
{
    GPASSERT(mTables.size() == mNeedDelete.size());
    int n = (int)(mTables.size());
    for (int i=0; i<n; ++i)
    {
        if (mNeedDelete[i])
        {
            delete mTables[i];
        }
    }
}

void GPMultiTable::addTable(IFunctionTable* table, bool need_delete)
{
    GPASSERT(NULL!=table);
    mTables.push_back(table);
    mNeedDelete.push_back(need_delete);
}
