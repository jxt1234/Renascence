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
#include "system/xmlGenerateSystem.h"
#include "system/system_lib.h"
#include "utils/debug.h"
#include <iostream>

using namespace std;

xmlGenerateSystem::xmlGenerateSystem()
{
    mComputeSystem = new computeSystem;
}

void xmlGenerateSystem::addXml(const char* xmlFile, IFunctionTable* table, bool print)
{
    xmlFunctionLoader xmlLoader;
    xmlLoader.loadFile(xmlFile);
    if (NULL==table)
    {
        table = new system_lib(xmlLoader.libName);
        mRemain.push_back(table);
    }
    mComputeSystem->loadFuncXml(xmlLoader, table, this);
    if (print)
    {
        xmlLoader.print();
        mComputeSystem->print(std::cout);
    }
    setComputeSystem(mComputeSystem);
}

xmlGenerateSystem::~xmlGenerateSystem()
{
    for (int i=0; i<mRemain.size(); ++i)
    {
        delete mRemain[i];
    }
    delete mComputeSystem;
}

std::string xmlGenerateSystem::xmlPrint(AbstractGP* gp)
{
    assert(NULL!=gp);
    vector<int> nullVector;
    gp->save(this, this, nullVector);
    return gp->xmlPrint(mComputeSystem, this);
}
