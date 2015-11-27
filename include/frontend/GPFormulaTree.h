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
#ifndef MATH_GPFORMULATREE_H
#define MATH_GPFORMULATREE_H
#include "utils/RefCount.h"
#include "math/GPAbstractPoint.h"
#include <string>
#include <ostream>
class GPFormulaTreePoint:public GPAbstractPoint
{
public:
    typedef enum
    {
        NUM,
        OPERATOR,
        ADF
    }TYPE;
    GPFormulaTreePoint();
    virtual ~GPFormulaTreePoint();
    friend class GPFormulaTree;
    inline const std::string& name() const {return mName;}
    inline TYPE type() const {return mT;}
    TYPE getChildType(size_t i) const;
    void render(std::ostream& output) const;
    void mergeForStatement(GPFormulaTreePoint* parent, size_t n);
    
    void valid() const;
private:
    TYPE mT;
    std::string mName;
};
class GPFormulaTree:public RefCount
{
public:
    GPFormulaTree();
    virtual ~GPFormulaTree();
    void setFormula(const std::string& formula);
    inline GPFormulaTreePoint* root() {return mRoot;}
private:
    GPFormulaTreePoint* mRoot;
};
#endif
