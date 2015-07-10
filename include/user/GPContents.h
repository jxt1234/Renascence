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
#ifndef USER_PACKAGE_H
#define USER_PACKAGE_H
#include <vector>
#include "IStatusType.h"

/*We can not define a destructor function for GPContents, because it's handlered by system*/
struct GPContents
{
    struct GP_Unit
    {
        void* content;
        const IStatusType* type;
    };
    std::vector<GP_Unit> contents;

    //Function for convinent
    void push(void* content, const IStatusType* type)
    {
        GP_Unit p;
        p.content = content;
        p.type = type;
        contents.push_back(p);
    }
    void push(const GP_Unit unit)
    {
        contents.push_back(unit);
    }
    void clearContents()
    {
        for (auto unit : contents)
        {
            if (unit.content && unit.type)
            {
                unit.type->vFree(unit.content);
            }
        }
    }
    void clear()
    {
        for (auto unit : contents)
        {
            if (unit.content && unit.type)
            {
                unit.type->vFree(unit.content);
            }
        }
        contents.clear();
    }
    inline void* get(size_t i) const {return contents[i].content;}
    inline const GP_Unit& operator[](size_t i) const {return contents[i];}
    inline size_t size() const {return contents.size();}
    static void destroy(GPContents* c)
    {
        c->clear();
        delete c;
    }
};

typedef GPContents*(*computeFunction)(GPContents* inputs);
#endif
