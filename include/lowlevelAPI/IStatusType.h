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
#ifndef USER_STATUS_H
#define USER_STATUS_H

#include <string>
#include <stdlib.h>
#include "GPStream.h"


/*Basic API*/
class IStatusType
{
public:
    IStatusType(const std::string name):mName(name){}
    virtual ~IStatusType(){}
    inline std::string name() const {return mName;}
    virtual void* vLoad(GPStream* input) const = 0;
    virtual void vSave(void* contents, GPWStream* output) const = 0;
    virtual void vFree(void* contents) const = 0;
    /* map
     * Modify contents by values.
     * return the number of parameters it needed.
     * If value is NULL, just return the number of parameters.
     * If *content is NULL and value is not NULL, alloc a new one.
     */
    virtual int vMap(void** content, double* value) const = 0;
    /* Check(Optional)
     * For Continue Data (Stream), Check if the data is completed, content must be not null
     */
    virtual bool vCheckCompleted(void* content) const {return NULL!=content;}
    /* Merge(Optional)
     * For Continue Data (Stream), Merge the src data to dst, dst and src must be not null
     * Normally, dst and src will be freed after calling this api
     * return NULL means can't merge
     */
    virtual void* vMerge(void* dst, void* src) const {return NULL;}

private:
    std::string mName;
};

class GPDoubleType:public IStatusType
{
public:
    GPDoubleType():IStatusType("double")
    {
    }
    virtual ~GPDoubleType()
    {
    }
    virtual void* vLoad(GPStream* input) const
    {
        return NULL;
    }
    virtual void vSave(void* contents, GPWStream* output) const
    {
        return;
    }
    virtual void vFree(void* contents) const
    {
        double* c = (double*)(contents);
        delete c;
    }
    virtual int vMap(void** content, double* value) const
    {
        return 0;
    }
};
extern IStatusType* gDefaultDoubleType;//For GP Self, Don't use it
class GPStringType:public IStatusType
{
public:
    GPStringType():IStatusType("String")
    {
    }
    virtual ~GPStringType()
    {
        
    }
    virtual void* vLoad(GPStream* input) const
    {
        return NULL;
    }
    virtual void vSave(void* contents, GPWStream* output) const
    {
        return;
    }
    virtual void vFree(void* contents) const
    {
        std::string* c = (std::string*)(contents);
        delete c;
    }
    virtual int vMap(void** content, double* value) const
    {
        return 0;
    }
};
extern IStatusType* gDefaultStringType;//For GP Self, Don't use it



typedef IStatusType*(*TYPECREATER)(const std::string& name);


#endif
