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
#include "frontend/GPFormulaTree.h"
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
                FINISHWORDS;
                break;
            case ',':
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

static void _removeUnusefulBracket(const std::vector<std::string>& words, int& sta, int& fin)
{
    int possible = 0;
    while (words[possible+sta] == "(" && words[fin-possible] == ")" && sta+possible < fin-possible) possible++;
    /*Determine if the bracket is matched*/
    while (possible > 0)
    {
        int depth = 0;
        bool valid = true;
        for (int i=sta+possible; i<fin-possible; ++i)
        {
            if (words[i] == "(")
            {
                depth++;
            }
            else if (words[i] == ")")
            {
                if (depth == 0)
                {
                    valid = false;
                    break;
                }
                depth--;
            }
        }
        if (valid)
        {
            break;
        }
        possible--;
    }
    sta = sta + possible;
    fin = fin - possible;
}

GPFormulaTreePoint::GPFormulaTreePoint(TYPE t, const std::string& name, GPFormulaTreePoint* father)
{
    mFather = father;
    mT = t;
    mName = name;
}


GPFormulaTreePoint::~GPFormulaTreePoint()
{
}

GPFormulaTreePoint* GPFormulaTreePoint::create(const std::vector<std::string>& words)
{
    return _create(words, 0, (int)words.size()-1, NULL);
}


static GPFormulaTreePoint* _mergeStringForPoint(const std::vector<std::string>& words, int sta, int fin, GPFormulaTreePoint* father)
{
    std::ostringstream output;
    for (int i=sta; i<=fin; ++i)
    {
        output << words[i];
    }
    GPFormulaTreePoint* child = new GPFormulaTreePoint(GPFormulaTreePoint::NUM, output.str(), father);
    return child;
}
GPFormulaTreePoint* GPFormulaTreePoint::_ParallelExpand(const std::vector<std::string>& words, int sta, int fin)
{
    _removeUnusefulBracket(words, sta, fin);
    switch (mChildren.size())
    {
        case 0://INPUT
        {
            GPFormulaTreePoint* p = new GPFormulaTreePoint(OPERATOR, "INPUT", this);
            p->_createSubPoints(words, sta, fin);
            return p;
        }
        case 1://Formula
        {
            return _create(words, sta, fin, this);
        }
        case 2://KeyMap
        {
            GPFormulaTreePoint* p= new GPFormulaTreePoint(OPERATOR, "KEYMAP", this);
            /*Find -> */
            for (int i=sta+1; i<fin; ++i)
            {
                if (words[i] == "->")
                {
                    GPFormulaTreePoint* input = new GPFormulaTreePoint(OPERATOR, "INPUTKEY", p);
                    input->_createSubPoints(words, sta, i-1);
                    GPFormulaTreePoint* output = new GPFormulaTreePoint(OPERATOR, "OUTPUTKEY", p);
                    output->_createSubPoints(words, i+1, fin);
                    p->addPoint(input);
                    p->addPoint(output);
                    break;
                }
            }
            GPASSERT(p->getChildrenNumber() > 0);
            return p;
        }
        case 3://Condition
        {
            return _mergeStringForPoint(words, sta, fin, this);
        }
        default:
            GPASSERT(0);
            break;
    }
    return NULL;
}

void GPFormulaTreePoint::_createSubPoint(const std::vector<std::string>& words, int sta, int fin)
{
    _removeUnusefulBracket(words, sta, fin);
    switch (mT) {
        case OPERATOR:
        case ADF:
            addPoint(_create(words, sta, fin, this));
            break;
        case PARALLEL:
            addPoint(_ParallelExpand(words, sta, fin));
            break;
        case NUM:
            addPoint(_mergeStringForPoint(words, sta, fin, this));
            break;
        default:
            GPASSERT(0);
            break;
    }
}

void GPFormulaTreePoint::_createSubPoints(const std::vector<std::string>& words, int sta, int fin)
{
    _removeUnusefulBracket(words, sta, fin);
    int pos = sta;
    int part_sta = pos;
    int depth = 0;
    for (; pos<=fin; ++pos)
    {
        auto w = words[pos];
        if (w == "(")
        {
            depth++;
        }
        else if (w == ")")
        {
            depth--;
        }
        else if (w == "," && depth == 0)
        {
            _createSubPoint(words, part_sta, pos-1);
            part_sta = pos+1;
        }
    }
    if (part_sta < pos)
    {
        _createSubPoint(words, part_sta, pos-1);
    }
    GPASSERT(depth == 0);
}


GPFormulaTreePoint* GPFormulaTreePoint::_create(const std::vector<std::string>& words, int sta, int fin, GPFormulaTreePoint* father)
{
    GPASSERT(sta<=fin);
    int depth = 0;
    bool multi = false;
    for (int i=sta; i<=fin; ++i)
    {
        auto w = words[i];
        if (w == "(")
        {
            depth++;
        }
        else if (w == ")")
        {
            depth--;
        }
        else if (w == "," && depth == 0)
        {
            multi = true;
            break;
        }
    }
    if (multi)
    {
        GPFormulaTreePoint* p = new GPFormulaTreePoint(OPERATOR, "SUB", father);
        p->_createSubPoints(words, sta, fin);
        return p;
    }
    
    GPASSERT(words[sta]!="(" && words[sta]!=")");
    if (sta == fin)
    {
        return new GPFormulaTreePoint(NUM, words[sta], father);
    }
    GPASSERT(words[fin]==")" && words[sta+1]=="(");
    GPFormulaTreePoint* root = NULL;
    if ("MAP" == words[sta] || "REDUCE" == words[sta])
    {
        root = new GPFormulaTreePoint(PARALLEL, words[sta], father);
    }
    else if ("ADF" == words[sta])
    {
        root = new GPFormulaTreePoint(ADF, words[sta], father);
    }
    else
    {
        root = new GPFormulaTreePoint(OPERATOR, words[sta], father);
    }
    sta = sta+2;
    fin = fin-1;
    root->_createSubPoints(words, sta, fin);
    return root;
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
    mRoot = GPFormulaTreePoint::create(words);
}

GPFormulaTreePoint::TYPE GPFormulaTreePoint::getChildType(size_t i) const
{
    GPASSERT(i>=0 && i<mChildren.size());
    auto p = (GPFormulaTreePoint*)mChildren[i];
    return p->type();
}

void GPFormulaTreePoint::render(std::ostream& output) const
{
    std::string type;
    switch (mT)
    {
        case NUM:
            type = "Num";
            break;
        case OPERATOR:
            type = "Func";
            break;
        case ADF:
            type = "ADF";
            break;
        case PARALLEL:
            type = "PARALLEL";
            break;
        default:
            GPASSERT(0);
            break;
    }
    output << "<"<<type << ">" << mName;
    if (!mChildren.empty())
    {
        output <<"\n";
    }
    for (auto p : mChildren)
    {
        GPCONVERT(GPFormulaTreePoint, p)->render(output);
    }
    output << "</"<<type << ">\n";
}

