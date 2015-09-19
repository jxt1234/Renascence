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
#ifndef MATH_FORMULATREE_H
#define MATH_FORMULATREE_H
#include "utils/RefCount.h"
#include "utils/AbstractPoint.h"
#include <string>
class FormulaTreePoint:public AbstractPoint
{
    public:
        typedef enum
        {
            NUM,
            OPERATOR
        }TYPE;
        FormulaTreePoint();
        virtual ~FormulaTreePoint();
        friend class FormulaTree;
        inline const std::string& name() const {return mName;}
        inline TYPE type() const {return mT;}
    private:
        TYPE mT;
        std::string mName;
};
class FormulaTree:public RefCount
{
    public:
        FormulaTree();
        virtual ~FormulaTree();
        void setFormula(const std::string& formula);
        inline FormulaTreePoint* root() {return mRoot;}
    private:
        FormulaTreePoint* mRoot;
};
#endif
