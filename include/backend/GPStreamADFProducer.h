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
#ifndef PRODUCER_GPSTREAMADFPRODUCER_H
#define PRODUCER_GPSTREAMADFPRODUCER_H
#include <stdio.h>
#include "backend/GPBackEndProducer.h"
class GPStreamADFProducer:public GPBackEndProducer
{
public:
    GPStreamADFProducer();
    virtual ~GPStreamADFProducer();
    virtual IGPAutoDefFunction* vCreateFromFuncTree(const GPFunctionTree* tree) const;
    virtual IGPAutoDefFunction* vCreateFromNode(const GPTreeNode* node, const GPFunctionDataBase* base) const;
};
#endif
