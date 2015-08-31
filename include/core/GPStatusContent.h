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
#ifndef CORE_GPSTATUSCONTENT_H
#define CORE_GPSTATUSCONTENT_H
#include "user/IStatusType.h"
#include "head.h"

class GPStatusContent:public RefCount
{
    public:
        GPStatusContent(const IStatusType* t, double defaultratio=0.5);
        GPStatusContent(const GPStatusContent& c);
        void operator=(const GPStatusContent& c);
        ~GPStatusContent();
        void print(std::ostream& out) const;
        void setValue(double* value, int n);
        inline void* content() const {return mContent;}
        inline const IStatusType* type() const {return mType;}
        inline int size() const{return mNumber;}
        void mutate();
    private:
        const IStatusType* mType;
        double* mValues;
        void* mContent;
        int mNumber;
};
#endif
