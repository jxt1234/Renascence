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

class GPFunctionDataBase:public RefCount
{
    public:
        struct function
        {
            //For print
            std::string name;
            std::string libName;
            //For compute
            computeFunction basic;
            std::vector<const IStatusType*> inputType;
            std::vector<const IStatusType*> outputType;
            std::vector<const IStatusType*> statusType;
            std::vector<std::vector<int> > fixTable;
        };
        computeFunction getFunction (int id) const;
        const function* getDetailFunction (int id) const;
        const function* getDetailFunction (const std::string& name) const;
        inline int getFunctionNumber() const{return mFunctionTable.size();}
        void loadXml(const char* file, IFunctionTable* table=NULL, std::ostream* print = NULL);
        void loadXml(std::istream& is, IFunctionTable* table=NULL, std::ostream* print = NULL);
        void clear();
        //Basic Api
        const std::vector<std::vector<int> >& getAvailableFunctionInputs(int functionId) const;
        std::vector<int> getOutputFunctions(const IStatusType* t) const;
        void print(std::ostream& os);
        const IStatusType* queryType(const std::string& name) const;
        GPFunctionDataBase();
        virtual ~GPFunctionDataBase();
    protected:
        void loadFuncXml(xmlFunctionLoader& loader, IFunctionTable* table);
        std::vector<const IStatusType*> loadStatus(const std::vector<xmlFunctionLoader::status>& sta, IFunctionTable* handle);
    private:
        void _loadUnit(xmlFunctionLoader func);
        std::vector<function*> mFunctionTable;
        std::vector<IFunctionTable*> mHandle;
        std::vector<IStatusType*> mTypes;
};

#endif
