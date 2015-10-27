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
#include <utils/GPDebug.h>
#include <sstream>
#include "math/GPFormulaTree.h"
#include <iostream>
#include <list>

static void divideFormula(std::vector<std::string>& results, const std::string& formula)
{
    int sta = 0;
    int fin = 0;
    bool startWords = false;
#define FINISHWORDS\
                if (startWords)\
                {\
                    fin = i;\
                    startWords = false;\
                    std::string _str;\
                    _str.assign(formula, sta, fin-sta);\
                    results.push_back(_str);\
                }

    for (int i=0;i<formula.size(); ++i)
    {
        char c = formula.at(i);
        if ('['==c||'{'==c) c='(';
        if (']'==c||'}'==c) c=')';
        switch (c)
        {
            case ' ':
            case '\t':
            case '\n':
            case ',':
                FINISHWORDS;
                break;
            case '(':
            case ')':
                FINISHWORDS;
                {
                    std::string _str;
                    _str.push_back(c);
                    results.push_back(_str);
                }
                break;
            default:
                if (!startWords)
                {
                    startWords = true;
                    sta = i;
                }
                break;
        }
    }
#undef FINISHWORDS
}
GPFormulaTreePoint::GPFormulaTreePoint()
{
}
GPFormulaTreePoint::~GPFormulaTreePoint()
{
}

GPFormulaTree::GPFormulaTree()
{
    mRoot = NULL;
}
GPFormulaTree::~GPFormulaTree()
{
    SAFE_UNREF(mRoot);
}
void GPFormulaTree::setFormula(const std::string& formula)
{
    SAFE_UNREF(mRoot);
    std::vector<std::string> words;
    divideFormula(words, formula);
    GPASSERT(!words.empty());

    /*Frist word is root*/
    mRoot = new GPFormulaTreePoint;
    mRoot->mT = GPFormulaTreePoint::NUM;
    mRoot->mName = words[0];

    GPFormulaTreePoint* current = mRoot;
    GPFormulaTreePoint* parent = NULL;
    std::list<GPFormulaTreePoint*> parentstack;
    for (int i=1; i<words.size(); ++i)
    {
        if ("(" == words[i])
        {
            current->mT = GPFormulaTreePoint::OPERATOR;
            parentstack.push_front(current);
            parent = parentstack.front();
        }
        else if (")" == words[i])
        {
            parentstack.pop_front();
            parent = NULL;
            if (!parentstack.empty())
            {
                parent = parentstack.front();
            }
        }
        else
        {
            current = new GPFormulaTreePoint;
            current->mName = words[i];
            current->mT = GPFormulaTreePoint::NUM;
            if (parent)
            {
                parent->addPoint(current);
            }
        }
    }
}

GPFormulaTreePoint::TYPE GPFormulaTreePoint::getChildType(size_t i) const
{
    GPASSERT(i>=0 && i<mChildren.size());
    auto p = (GPFormulaTreePoint*)mChildren[i];
    return p->type();
}
