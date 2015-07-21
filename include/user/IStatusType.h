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
#include <istream>
#include <ostream>


/*Basic API*/
class IStatusType
{
public:
    IStatusType(const std::string name):mName(name){}
    virtual ~IStatusType(){}
    inline std::string name() const {return mName;}
    virtual void* vLoad(std::istream& input) const = 0;
    virtual void vSave(void* contents, std::ostream& output) const = 0;
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
     * Normally, src will be freed after calling this api
     * return true if the merge is success, return false means can't merge
     */
    virtual bool vMerge(void* dst, void* src) const {return false;}

private:
    std::string mName;
};

typedef IStatusType*(*TYPECREATER)(const std::string& name);


#endif
