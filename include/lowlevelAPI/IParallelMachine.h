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

    class Creator : public GPRefCount
    {
    public:
        virtual ~Creator(){}
        virtual GPPieces* vPrepare(GPPieces** inputs, int inputNumber) const = 0;
    protected:
        Creator(){}
    };
    class Executor : public GPRefCount
    {
    public:
        virtual ~Executor(){}
        virtual bool vRun(GPPieces* output, GPPieces** inputs, int inputNumber) const = 0;
    protected:
        Executor(){}
    };

    /*Basic API*/
    virtual std::pair<Creator*, Executor*> vGenerate(const GPParallelType* data, PARALLELTYPE type) const = 0;

    /*For Parameter adjust*/
    virtual int vMap(GPFLOAT* values) {return 0;}

    typedef enum {
        INPUT,
        CACHE,
        OUTPUT
    } USAGE;
    
    /*Create Pieces from store system, or give a reference.
     *If keys = NULL and keyNum = 0, the IParallelMachine should determine it by description.
     *If don't support, return NULL*/
    virtual GPPieces* vCreatePieces(const char* description, std::vector<const IStatusType*> types, unsigned int* keys, int keyNum, USAGE usage) const = 0;
    
    /*Map the pieces created outside to the machine self
     *If no need to map, return NULL
     */
    virtual GPPieces* vMapPieces(GPPieces* outsidePieces) const = 0;

    virtual ~IParallelMachine(){}
protected:
    IParallelMachine(){}
};

typedef IParallelMachine*(*PARALLELMACHINE_CREATE_FUNC)(const std::string& name);

#endif
