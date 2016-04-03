/******************************************************************
   Copyright 2016, Jiang Xiao-tang

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
//
//  GPBlock.cpp
//  GP
//
//  Created by jiangxiaotang on 15/7/28.
//  Copyright (c) 2015年 jiangxiaotang. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include "utils/GPDebug.h"
#include "utils/GPBlock.h"
#define ALIGN(x) (((x)+7)/8*8)
GPBlock::GPBlock(size_t size)
{
    GPASSERT(0<size);
    mMem = new char[ALIGN(size)];
    mSize = size;
}
GPBlock::~GPBlock()
{
    delete [] mMem;
    mMem = NULL;
}
void GPBlock::setNext(GPPtr<GPBlock> block)
{
    mNext = block;
}
GPBlock* GPBlock::merge(GPBlock* b)
{
    GPASSERT(NULL!=b);
    /*Compute size*/
    size_t sum = 0;
    const GPBlock* iter = b;
    while (NULL!=iter)
    {
        sum+=iter->size();
        iter = iter->getNext();
    }
    auto res = new GPBlock(sum+1);
    iter = b;
    auto rescontent = res->contents();
    while (NULL!=iter)
    {
        auto content = iter->contents();
        ::memcpy(rescontent, content, iter->size());
        iter = iter->getNext();
    }
    rescontent[sum] = 0;
    return res;
}

void GPBlock::resize(size_t newsize)
{
    GPASSERT(newsize<=mSize && 0<newsize);
    mSize = newsize;
}
