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
#include "tinyxml.h"

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
    GPPtr<GPStream> filestream = GPStreamFactory::NewStream(file);
    loadPackage(filestream.get());
    this->attributeUnflatten();
    return mAttributes;
}

static GPTreeNode* _convert(const TiXmlNode* tiNode)
{
    auto name = tiNode->Value();
    auto root = new GPTreeNode(name);
    for (auto c = tiNode->FirstChild(); c!=NULL; c=c->NextSibling())
    {
        root->addChild(_convert(c));
    }
    return root;
}

GPTreeNode* xmlReader::loadPackage(GPStream* input)
{
    string tempLine;
    GPPtr<GPBlock> content = GPStreamUtils::read(input, true);
    const char* c = content->contents();
    TiXmlDocument document;
    document.Parse(c);
    mAttributes = _convert(document.FirstChild());
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
    cout << "Has "<<p->getChildren().size()<<" children"<<endl;
    for (auto c:p->getChildren())
    {
        printUnit(c.get());
    }
}

