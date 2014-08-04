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
#ifndef GP_COMPUTE_SYSTEM_H
#define GP_COMPUTE_SYSTEM_H
#include "core/function.h"
#include "core/status.h"
#include "xml/xmlFunctionLoader.h"
#include <string>
#include "core/IDataBase.h"
#include "user/IFunctionTable.h"
#include <ostream>

class computeSystem:public IPrintDataBase, public IGenerateDataBase
{
    public:
        struct function
        {
            //For print
            std::string name;
            std::string libName;
            //For compute
            computeFunction basic;
            std::vector<int> inputType;
            std::vector<int> outputType;
            std::vector<int> statusType;
            std::vector<std::vector<int> > fixTable;
        };
        computeFunction getFunction(int id);
        int getStatusId(int id);
        const function& getDetailFunction(int id);
        inline int getFunctionNumber(){return mFunctionTable.size();}
        void loadFuncXml(xmlFunctionLoader& loader, IFunctionTable* table, statusBasic* stadata);
        void clear();
        //Basic Api
        const std::vector<std::vector<int> >& getAvailableFunctionInputs(int functionId);
        std::vector<int> getOutputFunctions(int typeId = -1);
        void print(std::ostream& os);
        std::vector<int> searchOutputForType(const std::string& outputType);
        //DataBase
        virtual int vQueryFuncId(const std::string& funcName);
        virtual void vQueryStatus(int id, std::string& name, std::string& libName);
        virtual void vQueryFunction(int id, std::string& name, std::string& libName);
        virtual void vQueryOutput(int functionId, std::vector<int>& output);
        computeSystem();
        virtual ~computeSystem();
    protected:
        std::vector<int> loadStatus(const std::vector<xmlFunctionLoader::status>& sta, IFunctionTable* handle, statusBasic* stadata);
    private:
        void _loadUnit(xmlFunctionLoader func);
        std::vector<function*> mFunctionTable;
        std::vector<int> mOutputId;//可做为根结点的函数Id
        std::vector<int> mInputId;//作为叶结点的函数Id
};

#endif
