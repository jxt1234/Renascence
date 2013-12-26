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
#ifndef XML_XMLGPLOADER_H
#define XML_XMLGPLOADER_H

#include "xmlReader.h"
#include "core/GP_XmlString.h"
#include "core/AbstractGP.h"

class xmlGPLoader:public xmlReader, AbstractGP, IRuntimeDataBase
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
        void _getStatusFunc(const std::string& name, statusLoadMethod& _load, statusVaryMethod& _free);
        int findStatus(std::string name);
        void* findLib(std::string name);
        int findFunc(std::string name);
        void unFlattenUnit(xmlReader::package* p);
        void _node(xmlReader::package* p);
        void _status(xmlReader::package* p);
        void _lib(xmlReader::package* p);
        void _children(xmlReader::package* p);
        void _func(xmlReader::package* p);
        void _result(xmlReader::package* p);
        void load(int num, int type);
        void* mCurrentHandle;
        AbstractGP* mCurrentPoint;
};




#endif
