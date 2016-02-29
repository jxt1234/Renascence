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
#ifndef MATH_GPSINGLETREE_H
#define MATH_GPSINGLETREE_H

#include <string>
#include <map>
#include <vector>
#include "utils/RefCount.h"
#include "GPAbstractPoint.h"
#include "GPSingleTreeFunction.h"
#include "head.h"

/*GPSingleTree is used to Generate single formula such as y=x*2+sin(cos(x))*/

class GPSinglePoint:public GPAbstractPoint
{
public:
    typedef enum {
        FUNCTION,
        INPUT,
        LEFT_BRACKET,
        RIGHT_BRACKET,
        DIVIDE
    } TYPE;
    GPSinglePoint(TYPE type, int func_or_input);
    virtual ~GPSinglePoint();
    GPFLOAT compute(GPFLOAT* inputs);
    class GPSinglePointCopy:public GPAbstractPoint::IPointCopy
    {
    public:
        virtual GPAbstractPoint* copy(GPAbstractPoint* src, bool&);
    };
    static GPSinglePoint* createFromFormula(const std::vector<std::pair<TYPE, int>>& words);
private:
    static GPSinglePoint* _createFromFormula(const std::vector<std::pair<TYPE, int>>& words, int sta, int fin);

    union{
        struct {
            GPFLOAT* pCache;
            int iFunc;
        }sFunc;
        int iInput;
    }mData;
    TYPE mType;
};

class GPSingleTree:public RefCount
{
public:
    GPSingleTree(GPPtr<GPSinglePoint> root);
    GPSingleTree(const GPSingleTree& tree);
    void operator=(const GPSingleTree& tree);
    virtual ~GPSingleTree();
    inline GPFLOAT compute(GPFLOAT* inputs){return mRoot->compute(inputs);}
    int len() const;
    static GPPtr<GPSingleTree> createFromFormula(const std::string& formula, const std::string& variable);

    static GPPtr<GPSingleTree> createFromFormula(const std::string& formula, const std::map<std::string, int>& variableMap);
private:
    GPPtr<GPSinglePoint> mRoot;
};

#endif
