#ifndef XML_XMLGPLOADER_H
#define XML_XMLGPLOADER_H

#include "xmlReader.h"
#include "core/GP_XmlString.h"
#include "core/GeneticProgram.h"

class xmlGPLoader:public xmlReader, GeneticProgram, IFunctionDataBase
{
    public:
        xmlGPLoader(){mCurrentPoint = NULL;}
        virtual computeFunction vGetCompute(int id) {return mFunctions[id];}
        virtual ~xmlGPLoader(){}
        void reset();
        GP_Output run();
    protected:
        virtual void attributeUnflatten();
        std::vector<std::string> mLibName;
        std::vector<void*> mHandles;
        std::vector<std::string> mFuncName;
        std::vector<computeFunction> mFunctions;
        std::vector<std::string> mStatusName;
        std::vector<int> mStatusType;
    private:
        int findStatus(std::string name);
        void* findLib(std::string name);
        int findFunc(std::string name);
        void unFlattenUnit(xmlReader::package* p);
        void _node(xmlReader::package* p);
        void _status(xmlReader::package* p);
        void _lib(xmlReader::package* p);
        void _children(xmlReader::package* p);
        void _func(xmlReader::package* p);
        void load(int num, int type);
        void* mCurrentHandle;
        GeneticPoint* mCurrentPoint;
};




#endif
