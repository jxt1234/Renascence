#ifndef XML_READER_FUNCTION_H
#define XML_READER_FUNCTION_H
#include "xml/xmlReader.h"

class xmlFunctionLoader:public xmlReader
{
    public:
        struct function
        {
            string name;
            vector<vector<int> > inputs;
            vector<int> statusType;
        };
        struct status
        {
            string name;
            string _alloc;
            string _free;
            string _vary;
            string _copy;
            string _print;
            string _load;
        };
        string libName;
        int mOutput;
        vector<int> mInputs;
        int mFit;
        inline int size(){return mFunctions.size();}
        vector<vector<int> > getCombo(int funcId);
        vector<int> getType(int funcId);
        const vector<function>& getFunc(){return mFunctions;}
        const vector<status>& getStatus(){return mStatus;}
        string getFuncName(int funcId){return mFunctions[funcId].name;}
        string getStatusName(int statusId){return mStatus[statusId].name;}
        void print();
        xmlFunctionLoader(){}
        ~xmlFunctionLoader(){}
    protected:
        vector<package*> loadMain(package* p);
        void loadFunc(package* p);
        void loadStatus(package* p);
        virtual void subClear();
        virtual void attributeUnflatten();
    private:
        int findFunction(string name);
        int findStatus(string name);
        inline bool validId(int funcId){return funcId < mFunctions.size() && funcId>=0;}
        void printFunc(function& func);
        void printStatus(status& sta);
        vector<function> mFunctions;
        vector<status> mStatus;
};

#endif
