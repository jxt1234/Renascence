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

    class Executor : public GPRefCount
    {
    public:
        virtual ~Executor(){}
        virtual bool vRun(GPPieces* output, GPPieces** inputs, int inputNumber) const = 0;
    protected:
        Executor(){}
    };


    /*For Parameter adjust*/
    virtual int vMap(GPFLOAT* values) {return 0;}

    /*Get Executor by Parallel Data
     *The API will be called when build ADF, so parallelMachine can run some optimization in this API
     */
    virtual Executor* vPrepare(const GPParallelType* data, PARALLELTYPE type) const = 0;


    typedef enum {
        INPUT,//Can only read
        CACHE,//Can read and write
        OUTPUT//Can only write
    } USAGE;

    /*Create Pieces from store system, or give a reference.
     *If don't support, return NULL
     * For INPUT
     * types and description is needed,
     * if keys = NULL and keyNum = 0, the IParallelMachine should determine it by description.
     *
     * For OUTPUT
     * description is needed for OUTPUT, keys and keyNum should be valid
     *
     * For CACHE
     * Only keys and keyNum is needed
     */
    virtual GPPieces* vCreatePieces(const char* description, std::vector<const IStatusType*> types, unsigned int* keys, int keyNum, USAGE usage) const = 0;
    
    /*Copy the contents of readPieces to the writePieces
     *If can not support, return false;
     *The key dimension of readPieces and writePieces must be the same
     */
    virtual bool vCopyPieces(GPPieces* readPieces, GPPieces* writePieces) const = 0;

    virtual ~IParallelMachine(){}
protected:
    IParallelMachine(){}
};

typedef IParallelMachine*(*PARALLELMACHINE_CREATE_FUNC)(const std::string& name);

#endif
