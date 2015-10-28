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
#ifndef CORE_GPFUNCTIONTREE_H
#define CORE_GPFUNCTIONTREE_H
#include "math/GPAbstractPoint.h"
#include "GPFunctionDataBase.h"
class GPFunctionTree:public GPAbstractPoint
{
public:
    GPFunctionTree(const GPFunctionDataBase::function* f, size_t inputNumber);
    virtual ~GPFunctionTree();
    inline const GPFunctionDataBase::function* function() const {return mF;}
    inline size_t inputNumber() const {return mInputNumber;}
    class Iter :public RefCount
    {
    public:
        Iter(){}
        virtual ~Iter(){}
        virtual GPFunctionTree* vCurrent() const= 0;
        virtual bool vNext() = 0;
    };
private:
    /*mF=NULL for input node, mInputNumber = -1 for function node*/
    const GPFunctionDataBase::function* mF;
    size_t mInputNumber;
};
#endif
