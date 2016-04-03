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
#include "lowlevelAPI/GPContents.h"
#include "lowlevelAPI/GPStream.h"
#include "lowlevelAPI/IFunctionTable.h"
#include "GPMultiTable.h"
#include "xml/xmlReader.h"
#include "head.h"
#include "GPFunction.h"

class GPFunctionDataBase:public GPRefCount
{
public:
    std::vector<const IStatusType*> queryType(const std::string& typelist);
    //Basic Api
    std::vector<const IStatusType*> getAllType() const {return mTypes;}
    std::vector<const GPFunction*> getAllFunctions() const;
    virtual const GPFunction* vQueryFunction(const std::string& name) const;
    virtual const GPFunction* vQueryFunctionByShortName(const std::string& name) const;
    virtual const IStatusType* vQueryType(const std::string& name) const;
    //TODO Find better way to organnize it
    void loadXml(GPStream* is, IFunctionTable* table=NULL, std::ostream* print = NULL);
    
    //Find GPFunction that has outputType of t
    virtual std::vector<const GPFunction*> vSearchFunction(const IStatusType* t) const;
    
    /*For debug*/
    void print(std::ostream& os);
    GPFunctionDataBase();
    virtual ~GPFunctionDataBase();
private:
    void _addInfo(const GPTreeNode* node, IFunctionTable* table, std::ostream* print);
    void _addFunction(GPFunction* warpf, const GPTreeNode* func);
    int _findFunction(const std::string& name);
    const IStatusType* _findAndLoadStatus(const std::string& name);
    void _clear();
    std::vector<GPFunction*> mFunctionTable;
    GPPtr<GPMultiTable> mHandle;
    std::vector<TYPECREATER> mTypeCreateFuncs;
    std::vector<const IStatusType*> mTypes;
};

#endif
