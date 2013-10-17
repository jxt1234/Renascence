#include "system/xmlGenerateSystem.h"
#include "system/system_lib.h"

xmlGenerateSystem::xmlGenerateSystem(const char* xmlFile, bool print)
{
    mHandle=NULL;
    xmlFunctionLoader xmlLoader;
    xmlLoader.loadFile(xmlFile);
    if (print) xmlLoader.print();
    mComputeSystem = new computeSystem;
    mComputeSystem->loadFuncXml(xmlLoader, mHandle);
}

xmlGenerateSystem::~xmlGenerateSystem()
{
    if (NULL!=mHandle) system_unload_lib(mHandle);
    delete mComputeSystem;
}


