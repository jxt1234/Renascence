#ifndef XML_READER_FUNCTION_H
#define XML_READER_FUNCTION_H
#include "xml/xmlReader.h"

class xmlFunctionLoader:public xmlReader
{
    public:
        struct function
        {
            std::string name;
            std::vector<std::vector<int> > inputs;
            std::vector<int> statusType;
        };
        struct status
        {
            std::string name;
            std::string _alloc;
            std::string _free;
            std::string _vary;
            std::string _copy;
            std::string _print;
            std::string _load;
        };
        std::string libName;
        int mOutput;
        std::vector<int> mInputs;
        int mFit;
        inline int size(){return mFunctions.size();}
        std::vector<std::vector<int> > getCombo(int funcId);
        std::vector<int> getType(int funcId);
        const std::vector<function>& getFunc(){return mFunctions;}
        const std::vector<status>& getStatus(){return mStatus;}
        std::string getFuncName(int funcId){return mFunctions[funcId].name;}
        std::string getStatusName(int statusId){return mStatus[statusId].name;}
        void print();
        xmlFunctionLoader(){}
        ~xmlFunctionLoader(){}
    protected:
        std::vector<package*> loadMain(package* p);
        void loadFunc(package* p);
        void loadStatus(package* p);
        virtual void subClear();
        virtual void attributeUnflatten();
    private:
        int findFunction(std::string name);
        int findStatus(std::string name);
        inline bool validId(int funcId){return funcId < mFunctions.size() && funcId>=0;}
        void printFunc(function& func);
        void printStatus(status& sta);
        std::vector<function> mFunctions;
        std::vector<status> mStatus;
};

#endif
