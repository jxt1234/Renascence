#ifndef XML_GENERATE_SYSTEM_H
#define XML_GENERATE_SYSTEM_H

#include "system/GenerateSystem.h"
#include "system/xmlDataBase.h"

class xmlGenerateSystem:public GenerateSystem
{
    public:
        xmlGenerateSystem(const char* xmlFile, bool print= false);
        virtual ~xmlGenerateSystem();
        std::string xmlPrint(GeneticProgram* gp);
    private:
        xmlDataBase mData;
        std::vector<void*> mHandle;
};



#endif
