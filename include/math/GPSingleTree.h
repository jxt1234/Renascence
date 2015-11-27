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
#include "utils/RefCount.h"
#include "GPAbstractPoint.h"

/*GPSingleTree is used to Generate single formula such as y=x*2+sin(cos(x))*/

typedef float GPFLOAT;

class GPIFloatFunction:public RefCount
{
    public:
        GPIFloatFunction(int size):mSize(){}
        virtual ~GPIFloatFunction(){}
        inline int size() const {return mSize;}
        virtual GPFLOAT compute(GPFLOAT* data) = 0;
    private:
        int mSize;
};

class GPIFloatFunctionSet:public RefCount
{
    public:
        GPIFloatFunction* randomSelect();
        void add(GPIFloatFunction* func);
        void clear();

        GPIFloatFunctionSet(){}
        virtual ~GPIFloatFunctionSet(){}
    private:
        std::vector<GPIFloatFunction*> mSet;
};

class GPSinglePoint:public GPAbstractPoint
{
    public:
        GPSinglePoint(GPIFloatFunction* f);
        virtual ~GPSinglePoint();
        GPFLOAT compute();
        class GPSinglePointCopy:public GPAbstractPoint::IPointCopy
        {
            public:
                virtual GPAbstractPoint* copy(GPAbstractPoint* src, bool&);
        };
    protected:
        virtual void printBefore(std::ostream& out){}
        virtual void printAfter(std::ostream& out){}
    private:
        GPFLOAT* mCache;
        GPIFloatFunction* mF;
};

class GPSingleTree:public RefCount
{
    public:
        GPSingleTree(const GPIFloatFunctionSet& set);
        GPSingleTree(const GPSingleTree& tree);
        void operator=(const GPSingleTree& tree);
        virtual ~GPSingleTree();
        inline GPFLOAT compute(){return mRoot->compute();}
        int len();
    protected:
        GPSinglePoint* mRoot;
        const GPIFloatFunctionSet& mSet;
};

#endif
