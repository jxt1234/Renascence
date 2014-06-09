/******************************************************************
   Copyright 2014, Jiang Xiao-tang

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
#ifndef XML_XMLGPDATALOADER_H
#define XML_XMLGPDATALOADER_H
#include "xmlReader.h"
#include "core/GPData.h"
#include "core/status.h"
class xmlGPDataLoader:public xmlReader
{
    public:
        xmlGPDataLoader(statusBasic& sys);
        virtual ~xmlGPDataLoader();
        inline GPData* get() const{return mData;}
    protected:
        virtual void attributeUnflatten();
    private:
        statusBasic& mSys;
        GPData* mData;
};
#endif
