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
#include "utils/GPTreeNode.h"
#include "utils/GPDebug.h"
GPTreeNode::GPTreeNode(const std::string& name)
{
    mName = name;
}
GPTreeNode::~GPTreeNode()
{
    mChildren.clear();
}

void GPTreeNode::addChild(GPPtr<GPTreeNode> n)
{
    mChildren.push_back(n);
}
void GPTreeNode::addChild(const std::string& name)
{
    mChildren.push_back(new GPTreeNode(name));
}

void GPTreeNode::addChild(const std::string& name, const std::string& attr)
{
    mChildren.push_back(new GPTreeNode(name, attr));
}


GPTreeNode::GPTreeNode(const std::string& name, const std::string& attr)
{
    mName = name;
    mChildren.push_back(new GPTreeNode(attr));
}

std::string GPTreeNode::attr() const
{
    if (mChildren.size()>1 || mChildren.empty())
    {
        return "";
    }
    return mChildren[0]->name();
}
