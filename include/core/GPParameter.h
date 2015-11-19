/******************************************************************
   Copyright 2015, Jiang Xiao-tang

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
#ifndef CORE_GPPARAMETERS_H
#define CORE_GPPARAMETERS_H
#include "head.h"
/*A Seriers of numbers that in [0, 1]*/
class GPParameter:public RefCount
{
public:
    GPParameter(int n, const PFLOAT* c = NULL);
    virtual ~GPParameter();
    inline int size() const {return mNum;}
    inline PFLOAT operator[](int n) const {return mContent[n];}
    inline PFLOAT get(int n) const {return mContent[n];}
    const PFLOAT* get() const {return mContent;}
    inline PFLOAT* attach() {return mContent;}
    void clear(PFLOAT f);
    void makeValid();
private:
    PFLOAT* mContent;
    int mNum;
};
#endif
