/******************************************************************
   Copyright 2013, Jiang Xiao-tang

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
******************************************************************/
#ifndef XML_READER_FUNCTION_H
#define XML_READER_FUNCTION_H
#include "xmlReader.h"

class xmlFunctionLoader:public xmlReader
{
    public:
        struct function
        {
            std::string name;
            std::vector<std::vector<int> > inputs;
            std::vector<int> statusType;
            std::vector<int> outputType;
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
