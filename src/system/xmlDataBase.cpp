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
#include "system/xmlDataBase.h"
#include <assert.h>
using namespace std;
void xmlDataBase::vQueryStatus(int id, std::string& name, std::string& libName)
{
    assert(id < mStatus.size() && id >=0);
    name = mStatus[id].first;
    int libid = mStatus[id].second;
    libName = mLibName[libid];
}
void xmlDataBase::vQueryFunction(int id, std::string& name, std::string& libName)
{
    assert(id < mFunctions.size() && id >=0);
    name = mFunctions[id].first;
    int libid = mFunctions[id].second;
    libName = mLibName[libid];
}

xmlDataBase::xmlDataBase()
{
}

xmlDataBase::~xmlDataBase()
{
}

void xmlDataBase::loadXml(xmlFunctionLoader& loader)
{
    const vector<xmlFunctionLoader::function>& funcs = loader.getFunc();
    int libId = mLibName.size();
    mLibName.push_back(loader.libName);
    for (int i=0; i<funcs.size(); ++i)
    {
        pair<string, int> p(funcs[i].name, libId);
        mFunctions.push_back(p);
    }
    const vector<xmlFunctionLoader::status>& stat = loader.getStatus();
    for (int i=0; i<stat.size(); ++i)
    {
        pair<string, int> p(stat[i].name, libId);
        mStatus.push_back(p);
    }
}
