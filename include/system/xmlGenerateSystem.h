#ifndef XML_GENERATE_SYSTEM_H
#define XML_GENERATE_SYSTEM_H

#include "system/GenerateSystem.h"

class xmlGenerateSystem:public GenerateSystem
{
    public:
        xmlGenerateSystem(const char* xmlFile, bool print= false);
        virtual ~xmlGenerateSystem();
};



#endif
