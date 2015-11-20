/******************************************************************
 Copyright 2013, Jiang Xiao-tang
 
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
#ifndef ALGORITHM_ABSTRACTPOINT_H
#define ALGORITHM_ABSTRACTPOINT_H

#include <vector>
#include <list>
#include <ostream>
#include "utils/RefCount.h"
#include "utils/GPDebug.h"
class GPAbstractPoint:public RefCount
{
public:
    /*This class will alloc a new GPAbstractPoint and copy the contents from src*/
    class IPointCopy
    {
    public:
        virtual GPAbstractPoint* copy(GPAbstractPoint* src) = 0;
    };
    GPAbstractPoint(){}
    /*Use for different types of tree to transform*/
    static GPAbstractPoint* deepCopy(GPAbstractPoint* src, IPointCopy* copy);
    std::vector<const GPAbstractPoint*> display() const;
    virtual ~GPAbstractPoint();
    void addPoint(GPAbstractPoint* p);
    inline size_t getChildrenNumber() const {return mChildren.size();}
    inline const GPAbstractPoint* getChild(int n) const
    {
        GPASSERT(n>=0 && n<mChildren.size());
        return mChildren[n];
    }
    bool isChildOf(const GPAbstractPoint* parent) const;
    void clearChildren();
    bool replace(GPAbstractPoint* oldPoint, GPAbstractPoint* newPoint);
protected:
    std::vector<GPAbstractPoint*> mChildren;
private:
    int _posOfChild(GPAbstractPoint* p) const;
};


#endif
