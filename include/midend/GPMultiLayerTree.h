/******************************************************************
   Copyright 2015, Jiang Xiao-tang

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
#ifndef MIDEND_GPMULTILAYERTREE_H
#define MIDEND_GPMULTILAYERTREE_H
#include "core/GPFunctionTree.h"
#include <map>
class GPMultiLayerTree:public RefCount
{
public:
    typedef std::map<int, GPPtr<GPFunctionTreePoint>> POINTS;
    GPMultiLayerTree(const GPFunctionTree* tree);
    virtual ~GPMultiLayerTree();
    inline const POINTS& layers() const {return mLayers;}
private:
    POINTS mLayers;
};


#endif
