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
#ifndef UTILS_GPBLOCK_H
#define UTILS_GPBLOCK_H
#include "RefCount.h"
#include <stdlib.h>
class GPBlock:public RefCount
{
public:
    GPBlock(size_t size);
    virtual ~GPBlock();
    void setNext(GPPtr<GPBlock> block);
    inline size_t size() const {return mSize;}
    void resize(size_t newsize);
    inline char* contents() const {return mMem;}
    const GPBlock* getNext() const {return mNext.get();}

    /*Create a new block with just one node, copy all contents from b*/
    /*Can only be used in small memory case, less than 10M*/
    static GPBlock* merge(GPBlock* b);
private:
    GPPtr<GPBlock> mNext;
    char* mMem;
    size_t mSize;
};
#endif
