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
#ifndef LOWLEVELAPI_IPARALLELMACHINE_H
#define LOWLEVELAPI_IPARALLELMACHINE_H
#include "GPPieces.h"
#include "GPParallelType.h"
class IParallelMachine
{
public:
    typedef enum{
        MAP,
        REDUCE
    }PARALLELTYPE;

    class Creator
    {
    public:
        virtual ~Creator(){}
        virtual GPPieces* vPrepare(GPPieces** inputs, int inputNumber) const = 0;
    protected:
        Creator(){}
    };
    class Executor
    {
    public:
        virtual ~Executor(){}
        virtual bool vRun(GPPieces* output, GPPieces** inputs, int inputNumber) const = 0;
    protected:
        Executor(){}
    };
    virtual std::pair<Creator*, Executor*> vGenerate(const GPParallelType* data, PARALLELTYPE type) const = 0;
};
#endif
