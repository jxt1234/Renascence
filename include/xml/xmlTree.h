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
#ifndef XML_XMLTREE_H
#define XML_XMLTREE_H

#include "utils/AbstractPoint.h"
#include "utils/GPTreeNode.h"
#include <string>
#include <vector>


class xmlTree:public AbstractPoint
{
public:
    struct type
    {
        std::string name;
        std::string content;
    };
    xmlTree(const GPTreeNode* node);
    virtual ~xmlTree(){}
    inline const std::string& func(){return mFunc;}
    inline const std::string& inputTypes(){return mInputTypes;}
    inline const std::string& childFlags(){return mChildFlags;}

    inline const std::vector<type>& status(){return mStatus;}
protected:
    void loadNode(const GPTreeNode* p);
    /*Variables*/
    std::string mFunc;
    std::string mInputTypes;
    std::string mChildFlags;
    std::vector<type> mStatus;
private:
    void loadValues(std::vector<type>& result, const GPTreeNode* p);
    void printValues(std::vector<type>& result, std::ostream& out);
};



#endif
