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
#ifndef CORE_GPDATA_H
#define CORE_GPDATA_H
#include "utils/RefCount.h"
#include "user/package.h"
#include "user/status.h"
#include "user/package.h"
#include <ostream>
#include <string>
class GPData:public RefCount
{
    public:
        GPData(const std::string& name);
        ~GPData();
        GPContents* expand() const;
        void addData(void* content, const IStatusType& type);
        void print(std::ostream& out) const;
        double compare(const GPContents& output);
    private:
        struct data
        {
            void* content;
            const IStatusType& type;
            data(void* c, const IStatusType& s):content(c), type(s){}
            ~data()
            {
                type.vFree(content);
            }
        };
        std::vector<data*> mData;
        std::string mName;
};
#endif
