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
#ifndef UTILS_GPTREENODE_H
#define UTILS_GPTREENODE_H
#include <string>
#include <map>
#include <vector>
#include "lowlevelAPI/GPRefCount.h"
class GPTreeNode:public GPRefCount
{
public:
    GPTreeNode(const std::string& name, const std::string& attr);
    virtual ~GPTreeNode();
    void addChild(GPPtr<GPTreeNode> n);
    void addChild(const std::string& name, const std::string& attr);
    inline void setAttributes(const std::string& attr){mAttr = attr;}
    inline const std::string& attr() const {return mAttr;}
    inline const std::vector<GPPtr<GPTreeNode> >& getChildren() const {return mChildren;}
    inline const std::string& name() const {return mName;}
private:
    std::string mName;
    std::string mAttr;
    std::vector<GPPtr<GPTreeNode> > mChildren;
};
#endif
