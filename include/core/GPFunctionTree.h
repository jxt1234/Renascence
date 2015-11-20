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
#include "GPParameter.h"
#include <map>
#include <ostream>
class GPFunctionTreePoint:public GPAbstractPoint
{
public:
    GPFunctionTreePoint(const GPFunctionDataBase::function* f, int inputNumber);
    virtual ~GPFunctionTreePoint();
    inline const GPFunctionDataBase::function* function() const {return mF;}
    inline int inputNumber() const {return mInputNumber;}
    
    std::vector<const IStatusType*> getInputTypes() const;
    
    class Iter :public RefCount
    {
    public:
        Iter(){}
        virtual ~Iter(){}
        virtual GPFunctionTreePoint* vCurrent() const= 0;
        virtual bool vNext() = 0;
    };
    
    bool equal(const GPFunctionTreePoint* point) const;
    size_t depth() const;
    void render(std::ostream& output) const;
private:
    void _getInputTypes(std::vector<const IStatusType*>& types) const;
    /*mF=NULL for input node, mInputNumber = -1 for function node*/
    const GPFunctionDataBase::function* mF;
    int mInputNumber;
};

class GPFunctionTree:public RefCount
{
public:
    GPFunctionTree(GPPtr<GPFunctionTreePoint> root, bool total=true);
    virtual ~GPFunctionTree();
    std::string dump() const;

    inline GPFunctionTreePoint* root() const {return mRoot.get();}
    
    /*This kind of tree can be changed by mapStructure, the subtree shouldn't be one subtree of points in mVariableSubTree, If mVariableSubTree contains mRoot, all subtree can't be add*/
    void addVariableSubTree(GPFunctionTreePoint* subtree);
    
    inline const std::vector<GPFunctionTreePoint*>& getVariable() const {return mVariableSubTree;}
    
    static GPFunctionTree* copy(const GPFunctionTree* origin);
    static GPFunctionTreePoint* copy(const GPFunctionTreePoint* origin);
private:
    GPPtr<GPFunctionTreePoint> mRoot;
    
    /*Important: the subtree can't has child-parent relation, if mRoot is put into mVariableSubTree, it can only has mRoot*/
    std::vector<GPFunctionTreePoint*> mVariableSubTree;
};

#endif
