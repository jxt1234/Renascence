#ifndef XML_READER_FUNCTION_H
#define XML_READER_FUNCTION_H
#include "xml/xmlReader.h"

class xmlFunctionLoader:public xmlReader
{
    public:
        int mOutput;
        vector<int> mInputs;
        int mFit;
        inline int size(){return mFunctions.size();}
        vector<vector<int> > getCombo(int funcId);
        vector<int> getType(int funcId);
        string getFuncName(int funcId){return mFunctions[funcId].name;}
        string getStatusName(int statusId){return mStatus[statusId];}
        void print();
        xmlFunctionLoader(){}
        ~xmlFunctionLoader(){}
    protected:
        vector<package*> loadMain(package* p);
        void loadFunc(package* p);
        virtual void subClear();
        virtual void attributeUnflatten();
    private:
        int findFunction(string name);
        int findStatus(string name);
        inline bool validId(int funcId){return funcId < mFunctions.size() && funcId>=0;}
        struct function
        {
            string name;
            vector<vector<int> > inputs;
            vector<int> statusType;
        };
        void printFunc(function& func);
        vector<function> mFunctions;
        vector<string> mStatus;
};

#endif
