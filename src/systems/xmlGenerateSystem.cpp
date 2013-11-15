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

using namespace std;

xmlGenerateSystem::xmlGenerateSystem(const char* xmlFile, bool print)
{
    void* handle = NULL;
    xmlFunctionLoader xmlLoader;
    xmlLoader.loadFile(xmlFile);
    if (print) xmlLoader.print();
    mComputeSystem = new computeSystem;
    mComputeSystem->loadFuncXml(xmlLoader, handle);
    mHandle.push_back(handle);
}

xmlGenerateSystem::~xmlGenerateSystem()
{
    for (int i=0; i<mHandle.size(); ++i)
    {
        if (NULL!=mHandle[i]) system_unload_lib(mHandle[i]);
    }
    delete mComputeSystem;
}

std::string xmlGenerateSystem::xmlPrint(GeneticProgram* gp)
{
    assert(NULL!=gp);
    vector<int> func = gp->getFuncId();
    vector<int> saveFunc;
    for (int i=0; i<func.size(); ++i)
    {
        vector<int> out;
        mComputeSystem->vQueryOutput(func[i], out);
        if (!out.empty())
        {
            saveFunc.push_back(func[i]);
        }
    }
    gp->save(this, saveFunc);
    return gp->xmlPrint(mComputeSystem);
}

std::string xmlGenerateSystem::xmlPrint(AbstractGP* gp)
{
    assert(NULL!=gp);
    vector<int> nullVector;
    gp->save(this, nullVector);
    return gp->xmlPrint(mComputeSystem);
}



