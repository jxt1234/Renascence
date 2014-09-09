/******************************************************************
   Copyright 2014, Jiang Xiao-tang

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
#ifndef SRC_PRODUCER_GPSEARCHTREE_H
#define SRC_PRODUCER_GPSEARCHTREE_H

#include "math/carryTree.h"
#include "core/GPTreeADF.h"
/*This class use inputType and outputType to determine the relationship of each function, thus construct a tree*/

class GPSearchTreePoint:public carryPoint
{
    public:
        typedef const GPFunctionDataBase::function FUNC;
        typedef std::vector<FUNC*> PAIRP;
        typedef std::vector<PAIRP> GROUP;
        typedef const IStatusType* TYPEP;
        GPSearchTreePoint(const GPFunctionDataBase* base, const std::vector<TYPEP>& output);
        virtual ~GPSearchTreePoint();
        GPTreeADFPoint* output() const;
    protected:
        virtual bool vGrow();
        virtual bool vNext();
    private:
        PAIRP _getDependFunction();
        const PAIRP& current();
        int mCur;
        int mParent;
        GROUP mGroup;
        GPSearchTreePoint* mDepend;
        const GPFunctionDataBase* mSys;
    friend class GPSearchTree;
};

class GPSearchTree:public carryTree<GPTreeADF*>
{
    public:
        GPSearchTree(GPSearchTreePoint* p) {mRoot = p;}
        virtual GPTreeADF* output();
        virtual bool readyToOutput();
};

#endif
