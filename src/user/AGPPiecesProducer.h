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
#ifndef USER_AGPPIECESPRODUCER_H
#define USER_AGPPIECESPRODUCER_H
#include "backend/GPPiecesFunctionCreator.h"
#include "lowlevelAPI/GPPiecesFunction.h"
class AGPPiecesProducer:public GPRefCount
{
public:
    AGPPiecesProducer(GPParallelMachineSet* set, AGPProducer* producer, GPPiecesFunctionCreator* creator)
    {
        set->addRef();
        mSet = set;
        producer->addRef();
        mProducer = producer;
        mCreator = creator;
        creator->addRef();
    }
    virtual ~AGPPiecesProducer()
    {
        mSet->decRef();
        mProducer->decRef();
        mCreator->decRef();
    }

    GPParallelMachineSet* get() const {return mSet;}
    AGPProducer* getProducer() {return mProducer;}
    GPPiecesFunctionCreator* getCreator() const {return mCreator;}
private:
    GPParallelMachineSet* mSet;
    AGPProducer* mProducer;
    GPPiecesFunctionCreator* mCreator;
};
#endif
