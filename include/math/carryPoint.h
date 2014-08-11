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
#ifndef CARRY_POINT_H
#define CARRY_POINT_H
#include <vector>
#include <stdlib.h>
class carryPoint
{
public:
    carryPoint():mDepend(NULL), mGrow(false){}
    virtual ~carryPoint(){}
    virtual bool vGrow() = 0;
    virtual bool vNext() = 0;
    bool grow();
    bool next();
    void clear();
    carryPoint* mDepend;
    std::vector<carryPoint*> mChild;
    bool mGrow;
};



#endif
