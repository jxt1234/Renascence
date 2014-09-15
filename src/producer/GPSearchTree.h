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
#include "math/carryGroup2.h"
#include "core/GPTreeADF.h"
/*This class use inputType and outputType to determine the relationship of each function, thus construct a tree*/

class GPSearchTreePoint:public carryPoint
{
    public:
        typedef const GPFunctionDataBase::function FUNC;
        typedef std::vector<FUNC*> FUNCTEAM;
        typedef std::vector<FUNCTEAM> GROUP;
        typedef const IStatusType* TYPEP;
        GPSearchTreePoint(const GPFunctionDataBase* base, FUNC* f, GPSearchTreePoint* depend);
        virtual ~GPSearchTreePoint();
        GPTreeADFPoint* output() const;
        bool invalid() const;
    protected:
        virtual bool vGrow();
        virtual bool vNext();
    private:
        FUNCTEAM _getDependFunction();
        static void _addTeam(const GROUP& origin, const FUNCTEAM& forbid, GROUP& output);
        carryGroup2<FUNC*> mGroup;
        FUNC* mF;
        const FUNCTEAM& current() const;
        const GPFunctionDataBase* mBase;
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
