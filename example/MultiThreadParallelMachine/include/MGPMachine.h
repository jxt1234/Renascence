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
#ifndef MGPMACHINE_H
#define MGPMACHINE_H
#include "lowlevelAPI/IParallelMachine.h"

class MGPMachine:public IParallelMachine
{
public:
    virtual std::pair<Creator*, Executor*> vGenerate(const GPParallelType* data, PARALLELTYPE type) const override;
    virtual GPPieces* vCreatePieces(const char* description, std::vector<const IStatusType*> types, unsigned int* keys, int keyNum, USAGE usage) const override {return NULL;}
    
    virtual GPPieces* vMapPieces(GPPieces* outsidePieces) const override {return NULL;}
};

extern "C"{
    IParallelMachine* libmthpGP_PARALLELMACHINE_CREATE_FUNC(const std::string& name);
};

#endif
