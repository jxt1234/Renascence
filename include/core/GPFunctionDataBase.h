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
#include "user/GPContents.h"
#include <string>
#include "core/IDataBase.h"
#include "user/IFunctionTable.h"
#include "xml/xmlReader.h"
#include "head.h"

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
            std::vector<std::vector<int> > fixTable;//TODO Delete this
        };
        std::vector<const IStatusType*> queryType(const std::string typelist);
        //Basic Api
        virtual const function* vQueryFunction(const std::string& name) const;
        //In Future may delete these two API
        virtual const function* vQueryFunctionById (int id) const;
        virtual int size() const;
        virtual const IStatusType* vQueryType(const std::string& name) const;
        void loadXml(const char* file, IFunctionTable* table=NULL, std::ostream* print = NULL);
        void loadXml(std::istream& is, IFunctionTable* table=NULL, std::ostream* print = NULL);
        //Find Function that has outputType of t
        virtual std::vector<const function*> vSearchFunction(const IStatusType* t) const;
        void print(std::ostream& os);
        GPFunctionDataBase();
        virtual ~GPFunctionDataBase();
    private:
        void _addFunction(GPFunctionDataBase::function* warpf, const xmlReader::package* func, IFunctionTable* table);
        int _findFunction(const std::string& name);
        const IStatusType* _findAndLoadStatus(const std::string& name, IFunctionTable* handle);
        void _clear();
        std::vector<function*> mFunctionTable;
        std::vector<IFunctionTable*> mHandle;
        std::vector<const IStatusType*> mTypes;
};

#endif
