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
#ifndef CORE_STATUS_H
#define CORE_STATUS_H

#include <vector>
#include <string>
#include <stdlib.h>
#include "head.h"


/*Basic API*/

class IStatusType
{
    public:
        IStatusType(const std::string name):mName(name){}
        virtual ~IStatusType(){}
        inline std::string name() const {return mName;}
        virtual void* Alloc() const = 0;
        virtual void Free(void* contents) const = 0;
        virtual void mutate(void* contents) const = 0;
        virtual void mapValue(void* contents, double value) const = 0;
        virtual void copy(void* src, void* dst) const = 0;
        virtual void print(std::ostream& out, void* contents) const = 0;
        virtual void* load(std::istream& in) const = 0;
    private:
        std::string mName;
};


class GPStatusContent:public RefCount
{
    public:
        GPStatusContent(const IStatusType* t);
        GPStatusContent(const IStatusType* t, std::istream& is);
        GPStatusContent(const GPStatusContent& c);
        void operator=(const GPStatusContent& c);
        ~GPStatusContent();

        //Mutate
        void mutate();
        void mapValue(double value);

        inline void* content() const {return mContent;}
        inline const IStatusType& type() const {return *mType;}
    private:
        const IStatusType* mType;
        void* mContent;
};


/*
class statusBasic
{
    public:
        struct content
        {
            IStatusType* type;
            void* data;
        };
        statusBasic(){}
        virtual ~statusBasic();
        int addType(IStatusType* type, bool del=false);
        void clearStatusSet();
        //Firstly find the empty position and alloc in it, return the id
        int allocSet(int type, void* content = NULL);
        int allocSet(IStatusType* t, void* content = NULL);
        //Return -1 if id not found, return id if success
        int copySet(int id);
        int mutateSet(int id);
        int freeSet(int id);
        //Return NULL if id not found
        void* queryContent(int id);
        //Return all NULL Type if not found
        const IStatusType& queryType(int typeId);
        const IStatusType& queryTypeBySetId(int id);
        int queryType(const std::string& name);
    private:
        //The content of IStatusType is assumed to be free outside
        std::vector<IStatusType*> mType;
        std::vector<IStatusType*> mDeleteType;
        std::vector<content> mContents;
};
*/
#endif
