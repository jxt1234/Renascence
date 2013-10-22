#include "system/xmlGenerateSystem.h"
#include "system/system_lib.h"

xmlGenerateSystem::xmlGenerateSystem(const char* xmlFile, bool print)
{
    void* handle = NULL;
    xmlFunctionLoader xmlLoader;
    xmlLoader.loadFile(xmlFile);
    if (print) xmlLoader.print();
    mComputeSystem = new computeSystem;
    mComputeSystem->loadFuncXml(xmlLoader, handle);
    mData.loadXml(xmlLoader);
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
    return gp->xmlPrint(&mData);
}


