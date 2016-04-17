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
#include "xml/xmlReader.h"
#include <list>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "core/GPStreamUtils.h"
#include "core/GPStreamFactory.h"
#include "utils/GPDebug.h"
#include "utils/AutoStorage.h"

using namespace std;

xmlReader::xmlReader()
{
    mAttributes = NULL;
    mCurPackage = NULL;
}
xmlReader::~xmlReader(){clear();}

void xmlReader::clear()
{
    delete mAttributes;
    mAttributes = NULL;
    mCurPackage = NULL;
    mString.clear();
}

XMLAPI const GPTreeNode* xmlReader::loadStream(GPStream* input)
{
    clear();
    loadPackage(input);
    this->attributeUnflatten();
    return mAttributes;
}

XMLAPI const GPTreeNode* xmlReader::loadFile(const char* file)
{
    GPASSERT(NULL!=file);
    clear();
    GPPtr<GPStreamWrap> filestream = GPStreamFactory::NewStream(file, GPStreamFactory::FILE);
    loadPackage(filestream.get());
    this->attributeUnflatten();
    return mAttributes;
}


static bool xmlReader_endPackage(const string& line, int pos)
{
    bool result =  line.find("</")!=string::npos;
    return result;
}

static bool xmlReader_startPackage(const string& line, int& cur)
{
    bool result = true;
    cur = line.find("<", cur);
    if (string::npos == cur || cur >=line.size())
    {
        cur = 0;
        result = false;
    }
    else
    {
        // "</"  Means End
        if (line[cur+1]=='/')
        {
            cur = 0;
            result = false;
        }
    }
    //cout <<line << " start "<<result<<endl;
    return result;
}

static string xmlReader_getPackageName(const string& line, int& sta)
{
    int cur = sta;
    string result;
    while (line[cur]!='>' && cur < line.size()) cur++;
    if (cur>=line.size()) return result;
    result = line.substr(sta+1, cur-sta-1);
    sta = cur+1;
    return result;
}


static string xmlReader_getAttribute(const string& line, int& pos)
{
    int newPos = line.find("</", pos);
    if (string::npos == newPos)
    {
        newPos = line.size();
    }
    string result = line.substr(pos, newPos-pos);
    pos = newPos;
    return result;
}

static bool xmlReader_divider(char c)
{
    return (c == ' ') || (c == '\t')||(c == ',');
}

vector<string> xmlReader_divideString(const string& line)
{
#define LENGTH_OUT(x) if (x >= line.size()) {break;}
    vector<string> result;
    int cur =0;
    while(true)
    {
        for (;xmlReader_divider(line[cur]) && cur < line.size();++cur);
        LENGTH_OUT(cur);
        int sta = cur;
        for (++cur;(!xmlReader_divider(line[cur]))&& cur < line.size();++cur);
        int fin = cur;
        result.push_back(line.substr(sta, fin-sta));
        LENGTH_OUT(cur);
        ++cur;
    }
#undef LENGTH_OUT
    return result;
}

void xmlReader::analysisLine(const string& line)
{
    string results = "";
    if (line.size()<=0) return;
    int cur = 0;
    if (xmlReader_startPackage(line, cur))
    {
        string name = xmlReader_getPackageName(line, cur);
        GPTreeNode* newpackage = new GPTreeNode(name, "");
        if (NULL!=mCurPackage)
        {
            mCurPackage->addChild(newpackage);
        }
        //Save current string to current package
        if (mString!="")
        {
            mCurPackage->setAttributes(mString);
        }
        mString.clear();
        //Turn to new package
        mCurPackage = newpackage;
        mPackage.push_back(mCurPackage);
    }
    //Get attribute
    mString = mString + xmlReader_getAttribute(line, cur);
    //Just if end meet
    if (xmlReader_endPackage(line, cur))
    {
        //Save current string to current package
        if ("" == mCurPackage->attr())
        {
            mCurPackage->setAttributes(mString);
        }
        mString.clear();
        //Turn to new package
        mPackage.pop_back();
        if (!mPackage.empty())
        {
            mCurPackage = *(mPackage.rbegin());
        }
    }
}

GPTreeNode* xmlReader::loadPackage(GPStream* input)
{
    string tempLine;
    GPPtr<GPBlock> content = GPStreamUtils::read(input, true);
    const char* c = content->contents();
    istringstream inp(c);
    while(getline(inp, tempLine, '\n'))
    {
        analysisLine(tempLine);
    }
    mAttributes = mCurPackage;
    attributeUnflatten();
    return mAttributes;
}

void xmlReaderTest::print()
{
    printUnit(mAttributes);
}

void xmlReader::dumpNodes(const GPTreeNode* node, GPWStream* output)
{
    ostringstream os;
    os << "<"<<node->name()<<">\n";
    os <<node->attr()<<"\n";
    output->vWrite(os.str().c_str(), os.str().size());
    for (auto c : node->getChildren())
    {
        dumpNodes(c.get(), output);
    }
    os.str("");
    os << "</"<<node->name()<<">\n";
    output->vWrite(os.str().c_str(), os.str().size());
}


void xmlReaderTest::printUnit(GPTreeNode* p)
{
    if (NULL == p) return;
    cout << endl<<"Name = "<<p->name() <<endl;
    cout << "attribute:"<<p->attr()<<endl;
    cout << "Has "<<p->getChildren().size()<<" children"<<endl;
    for (auto c:p->getChildren())
    {
        printUnit(c.get());
    }
}

