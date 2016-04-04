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
#include "lowlevelAPI/IParallelMachine.h"
#include "math/GPAbstractPoint.h"
#include "GPFunctionDataBase.h"
#include "GPParameter.h"
#include <map>
#include <string>
#include <ostream>
class GPFunctionTreePoint:public GPAbstractPoint
{
public:
    typedef enum {
        FUNCTION,
        INPUT,
        PARALLEL,
        STRING
    } TYPE;
    GPFunctionTreePoint(const GPFunction* f);
    GPFunctionTreePoint(TYPE t, int iData);
    GPFunctionTreePoint(const GPFunctionTreePoint& p);
    GPFunctionTreePoint(const std::string& extra);
    virtual ~GPFunctionTreePoint();
    
    typedef union {
        const GPFunction* pFunc;
        int iInput;
        IParallelMachine::PARALLELTYPE iParallelType;//0: MAP, 1: REDUCE
    } DATA;
    inline const DATA data() const {return mData;}
    inline TYPE type() const {return mType;}
    inline const std::string& extra() const {return mExtraString;}
    void copyFrom(const GPFunctionTreePoint* src);
    
    std::map<int, const IStatusType*> getInputTypes() const;
    
    void mapInput(const std::map<int, int>& inputMap);
    void mapInput(const std::map<int, GPFunctionTreePoint*>& inputMap);
    bool equal(const GPFunctionTreePoint* point) const;
    int maxInputPos() const;
    size_t depth() const;
    void render(std::ostream& output) const;
    
    void valid() const;
private:
    bool _equal(const GPFunctionTreePoint* p) const;
    void _getInputTypes(std::map<int, const IStatusType*>& types) const;
    void _renderChildren(std::ostream& output) const;
    DATA mData;
    std::string mExtraString;
    TYPE mType;
};

class GPFunctionTree:public GPRefCount
{
public:
    GPFunctionTree(GPPtr<GPFunctionTreePoint> root, bool total=true);
    virtual ~GPFunctionTree();
    std::string dump() const;

    inline GPFunctionTreePoint* root() const {return mRoot.get();}
    
    /*This kind of tree can be changed by mapStructure, the subtree shouldn't be one subtree of points in mVariableSubTree, If mVariableSubTree contains mRoot, all subtree can't be add*/
    void addVariableSubTree(GPFunctionTreePoint* subtree, const std::vector<GPFunctionTreePoint*>& immutable, const std::string& name);
    
    inline const std::map<GPFunctionTreePoint*, std::vector<GPFunctionTreePoint*>>& getVariable() const {return mVariableSubTree;}
    inline const std::map<std::string, GPFunctionTreePoint*>& getSubTreeName() const {return mSubTreeName;}
    
    static GPFunctionTree* copy(const GPFunctionTree* origin);
    static GPFunctionTreePoint* copy(const GPFunctionTreePoint* origin);
private:
    GPPtr<GPFunctionTreePoint> mRoot;
    
    /*Important: the subtree can't has child-parent relation, if mRoot is put into mVariableSubTree, it can only has mRoot*/
    std::map<GPFunctionTreePoint*, std::vector<GPFunctionTreePoint*>> mVariableSubTree;
    std::map<std::string, GPFunctionTreePoint*> mSubTreeName;
};

#endif
