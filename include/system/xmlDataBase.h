#ifndef SYSTEM_XMLDATABASE_H
#define SYSTEM_XMLDATABASE_H

#include "core/IDataBase.h"
#include "xml/xmlFunctionLoader.h"

class xmlDataBase:public IDataBase
{
    public:
        /*API for GP*/
        virtual void vQueryStatus(int id, std::string& name, std::string& libName);
        virtual void vQueryFunction(int id, std::string& name, std::string& libName);
        /*Load Data Method*/
        void loadXml(xmlFunctionLoader& loader);
        void loadXml(const char* fileName);

        xmlDataBase();
        virtual ~xmlDataBase();
    protected:
        std::vector<std::pair<std::string, int> > mFunctions;
        std::vector<std::pair<std::string, int> > mStatus;
        std::vector<std::string> mLibName;
};

#endif
