#include "system/xmlGenerateSystem.h"
xmlGenerateSystem::xmlGenerateSystem(const char* xmlFile, bool print)
{
    xmlFunctionLoader xmlLoader;
    xmlLoader.loadFile(xmlFile);
    if (print) xmlLoader.print();
    mComputeSystem = new computeSystem;
    mComputeSystem->loadFuncXml(xmlLoader);
    mStatusSystem = new statusSystem;
    mStatusSystem->loadFuncXml(xmlLoader);
}

xmlGenerateSystem::~xmlGenerateSystem()
{
    delete mComputeSystem;
    delete mStatusSystem;
}


