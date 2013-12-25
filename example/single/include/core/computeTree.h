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
#ifndef COMPUTE_TREE_H
#define COMPUTE_TREE_H
#include "utils/BasicTree.h"
#include "function.h"
struct computePoint
{
    bool saved;
    vector<void*> output;
    computeFunction func;
    vector<void*> status;
    vector<computePoint*> childern;
};

class computeTree:public BasicTree<computePoint>
{
    public:
        computeTree(){}
        virtual ~computeTree(){}
};

#endif
