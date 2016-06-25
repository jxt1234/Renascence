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
#include "math/GPSingleTree.h"
#include <utils/GPDebug.h>
#include <stdlib.h>
#include <sstream>
using namespace std;
GPSinglePoint* GPSinglePoint::createFromFormula(const vector<pair<TYPE, int>>& words)
{
    return _createFromFormula(words, 0, (int)words.size()-1);
}

GPSinglePoint* GPSinglePoint::_createFromFormula(const vector<pair<TYPE, int>>& words, int sta, int fin)
{
    GPASSERT(words.size()>0);
    for (; sta<fin; sta++, fin--)
    {
        if (words[sta].first != LEFT_BRACKET || words[fin].first!=RIGHT_BRACKET)
        {
            break;
        }
    }
    GPASSERT(words[sta].first == FUNCTION || words[sta].first == INPUT);
    if (sta == fin)
    {
        GPASSERT(words[sta].first == INPUT);
        return new GPSinglePoint(INPUT, words[sta].second);
    }
    /*Function with bracket*/
    if (words[sta].first == FUNCTION && words[fin].first == RIGHT_BRACKET)
    {
        int func = words[sta].second;
//        GPASSERT(words[fin].first == RIGHT_BRACKET);
        GPASSERT(words[sta+1].first == LEFT_BRACKET);
        sta = sta + 1;//Move to LEFT_BRACKET
        GPSinglePoint* root = new GPSinglePoint(FUNCTION, func);
        int depth = 0;
        int substa = sta;
        int subfin = -1;
        for (int i=sta+1; i<fin; ++i)
        {
            switch (words[i].first) {
                case LEFT_BRACKET:
                    depth++;
                    break;
                case RIGHT_BRACKET:
                    depth--;
                    break;
                case DIVIDE:
                    if (depth == 0)
                    {
                        subfin = i-1;
                        root->addPoint(_createFromFormula(words, substa, subfin));
                        substa = i+1;
                    }
                    break;
                default:
                    break;
            }
        }
        GPASSERT(depth == 0);
        root->addPoint(_createFromFormula(words, substa, fin));
        GPASSERT(root->getChildrenNumber() == GPSingleTreeFunction::inputNumber(func));
        return root;
    }
    /*formula like a+b */
    /*Find Mid*/
    int mid = sta;
    int prior = 1000;//Max prior
    int depth = 0;
    for (int i=sta+1; i<fin; ++i)
    {
        switch (words[i].first) {
            case LEFT_BRACKET:
                depth++;
                break;
            case RIGHT_BRACKET:
                depth--;
                break;
            case FUNCTION:
                if (depth == 0)
                {
                    int p = GPSingleTreeFunction::priority(words[i].second);
                    if (p <= prior)
                    {
                        mid = i;
                        prior = p;
                    }
                }
                break;
            default:
                break;
        }
    }
    GPASSERT(mid != sta);
    GPASSERT(words[mid].first == FUNCTION);
    GPSinglePoint* midpoints = new GPSinglePoint(FUNCTION, words[mid].second);
    midpoints->addPoint(_createFromFormula(words, sta, mid-1));
    midpoints->addPoint(_createFromFormula(words, mid+1, fin));
    return midpoints;
}


GPSinglePoint::GPSinglePoint(TYPE type, int func_or_input)
{
    mType = type;
    switch (type) {
        case FUNCTION:
        {
            mData.sFunc.iFunc = func_or_input;
            mData.sFunc.pCache = NULL;
            int number = GPSingleTreeFunction::inputNumber(func_or_input);
            if (number > 0)
            {
                mData.sFunc.pCache = new GPFLOAT[number];
                GPASSERT(NULL!=mData.sFunc.pCache);
            }
            break;
        }
        case INPUT:
            mData.iInput = func_or_input;
            break;
        default:
            break;
    }
}

GPSinglePoint::~GPSinglePoint()
{
    if (FUNCTION == mType && NULL!=mData.sFunc.pCache)
    {
        delete [] mData.sFunc.pCache;
    }
}

GPFLOAT GPSinglePoint::compute(GPFLOAT* inputs)
{
    if (INPUT == mType)
    {
        return inputs[mData.iInput];
    }
    for (int i=0; i<mChildren.size(); ++i)
    {
        GPSinglePoint* p = (GPSinglePoint*)(mChildren[i]);
        mData.sFunc.pCache[i] = p->compute(inputs);
    }
    return GPSingleTreeFunction::compute(mData.sFunc.iFunc, mData.sFunc.pCache);
}

GPAbstractPoint* GPSinglePoint::GPSinglePointCopy::copy(GPAbstractPoint* src, bool&)
{
    GPSinglePoint* s = (GPSinglePoint*)src;
    GPSinglePoint* res = NULL;
    switch (s->mType)
    {
        case FUNCTION:
            res = new GPSinglePoint(s->mType, s->mData.sFunc.iFunc);
            break;
        case INPUT:
            res = new GPSinglePoint(s->mType, s->mData.iInput);
            break;
        default:
            break;
    }
    return (GPAbstractPoint*)res;
}


GPSingleTree::GPSingleTree(GPPtr<GPSinglePoint> root)
{
    GPASSERT(NULL!=root.get());
    mRoot = root;
}


GPSingleTree::GPSingleTree(const GPSingleTree& tree)
{
    GPSinglePoint::GPSinglePointCopy c;
    mRoot = (GPSinglePoint*)GPAbstractPoint::deepCopy(tree.mRoot.get(), &c);
}

void GPSingleTree::operator=(const GPSingleTree& tree)
{
    GPSinglePoint::GPSinglePointCopy c;
    mRoot = (GPSinglePoint*)GPAbstractPoint::deepCopy(tree.mRoot.get(), &c);
}

GPSingleTree::~GPSingleTree()
{
}
int GPSingleTree::len() const
{
    vector<const GPAbstractPoint*> list = mRoot->display();
    return (int)(list.size());
}

std::vector<std::string> GPSingleTree::divideWords(const std::string& formula)
{
    /*Divide words*/
    int sta = 0;
    int fin = 0;
    bool startWords = false;
    vector<string> results;
#define FINISHWORDS(i)\
if (startWords)\
{\
fin = i;\
startWords = false;\
std::string _str;\
_str.assign(formula, sta, fin-sta);\
results.push_back(_str);\
}
    
    for (int i=0;i<formula.size();)
    {
        int len = GPSingleTreeFunction::searchFunction(i, formula);
        if (len > 0)
        {
            FINISHWORDS(i);
            {
                std::string _str;
                _str.assign(formula, i, len);
                results.push_back(_str);
            }
            i += len;
            continue;
        }
        char c = formula.at(i);
        if ('['==c||'{'==c) c='(';
        if (']'==c||'}'==c) c=')';
        switch (c)
        {
            case ' ':
            case '\t':
            case '\n':
            case ',':
                FINISHWORDS(i);
                break;
            case '(':
            case ')':
                FINISHWORDS(i);
            {
                std::string _str(1, c);
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
        ++i;
    }
    FINISHWORDS((formula.size()));
#undef FINISHWORDS
    return results;
}


GPSingleTree* GPSingleTree::createFromFormula(const std::string& formula, const std::map<std::string, int>& variableMap)
{
    auto results = divideWords(formula);
    vector<pair<GPSinglePoint::TYPE, int>> words;
    for (int i=0; i<results.size(); ++i)
    {
        if (results[i] == "(")
        {
            words.push_back(make_pair(GPSinglePoint::LEFT_BRACKET, -1));
            continue;
        }
        if (results[i] == ")")
        {
            words.push_back(make_pair(GPSinglePoint::RIGHT_BRACKET, -1));
            continue;
        }
        int p = GPSingleTreeFunction::func(results[i]);
        if (p>=0)
        {
            words.push_back(make_pair(GPSinglePoint::FUNCTION, p));
            continue;
        }
        auto pos = variableMap.find(results[i]);
        GPASSERT(pos != variableMap.end());
        words.push_back(make_pair(GPSinglePoint::INPUT, pos->second));
    }
    
    return new GPSingleTree(GPSinglePoint::createFromFormula(words));
}

GPSingleTree* GPSingleTree::createFromFormula(const std::string& formula, const std::string& variable)
{
    istringstream input(variable);
    std::map<std::string, int> variableMap;
    int pos = 0;
    std::string temp;
    while (input >> temp)
    {
        variableMap[temp] = pos;
        pos++;
    }
    return createFromFormula(formula, variableMap);
}
