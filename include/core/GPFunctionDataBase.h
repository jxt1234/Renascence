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
#include "user/GPStream.h"
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
        std::string shortname;
        //For compute
        computeFunction basic;
        std::vector<const IStatusType*> inputType;
        std::vector<const IStatusType*> outputType;
        std::vector<const IStatusType*> statusType;
        std::vector<std::vector<const function*> > fixTable;//TODO Delete this
    };
    std::vector<const IStatusType*> queryType(const std::string& typelist);
    //Basic Api
    std::vector<const function*> getAllFunctions() const;
    virtual const function* vQueryFunction(const std::string& name) const;
    virtual const function* vQueryFunctionByShortName(const std::string& name) const;
    virtual const IStatusType* vQueryType(const std::string& name) const;
    //TODO Find better way to organnize it
    void loadXml(GPStream* is, IFunctionTable* table=NULL, std::ostream* print = NULL);
    
    //Find Function that has outputType of t
    virtual std::vector<const function*> vSearchFunction(const IStatusType* t) const;
    
    /*For debug*/
    void print(std::ostream& os);
    GPFunctionDataBase();
    virtual ~GPFunctionDataBase();
private:
    void _addFunction(GPFunctionDataBase::function* warpf, const GPTreeNode* func, IFunctionTable* table);
    int _findFunction(const std::string& name);
    const IStatusType* _findAndLoadStatus(const std::string& name, IFunctionTable* handle);
    void _clear();
    std::vector<function*> mFunctionTable;
    std::vector<IFunctionTable*> mHandle;
    std::vector<const IStatusType*> mTypes;
};

#endif
