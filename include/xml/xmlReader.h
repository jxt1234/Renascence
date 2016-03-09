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
#ifndef XML_READER_H
#define XML_READER_H
#include "lowlevelAPI/GPStream.h"
#include <string>
#include <vector>
#include <istream>
#include "utils/GPTreeNode.h"

#define XMLAPI


class xmlReader
{
public:
    XMLAPI const GPTreeNode* loadFile(const char* file);
    XMLAPI const GPTreeNode* loadStream(GPStream* input);
    XMLAPI void writeFile(const char* file);
    static void dumpNodes(const GPTreeNode* node, GPWStream* output);
    xmlReader();
    virtual ~xmlReader();
protected:
    virtual void attributeUnflatten() {}
    void clear();
    //For child class to clear their self data
    virtual void subClear(){}
    void analysisLine(const std::string& line);
    GPTreeNode* loadPackage(GPStream* input);
    GPTreeNode* mAttributes;
private:
    std::vector<GPTreeNode*> mPackage;
    std::string mString;
    GPTreeNode* mCurPackage;
};

class xmlReaderTest:public xmlReader
{
public:
    XMLAPI void print();
    xmlReaderTest(){}
    virtual ~xmlReaderTest(){}
protected:
    void printUnit(GPTreeNode* p);
    virtual void attributeUnflatten(){print();}
};

#endif
