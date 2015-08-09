/******************************************************************
   Copyright 2014, Jiang Xiao-tang

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

#include "FilePath.h"
#include "user/GPAPI.h"
#include <sstream>

using namespace std;
struct typeString
{
    int type;
    string content;
};

static typeString gMap[] = {
    {FilePath::RUNTIME, string("runtime.xml")},
    {FilePath::COMPARE, string("compare.xml")},
    {FilePath::INPUT, string("inputTable.txt")},
    {FilePath::OUTPUT, string("outputTable.txt")},
    {FilePath::AUTOMACHINE, string("autoMachine.xml")},
    {FilePath::STANDARD, string("standard.xml")},
    {FilePath::RESULT, string("result.txt")}
};


string FilePath::file(FilePath::TYPE t)
{
    const int l = sizeof(gMap)/sizeof(typeString);
    ostringstream os;
    for (int i=0; i<l; ++i)
    {
        if (gMap[i].type == t)
        {
            os << gMap[i].content;
            break;
        }
    }
    return os.str();
}

GPStream* FilePath::open(FilePath::TYPE t)
{
    string name = file(t);
    return GP_Stream_Create(name.c_str());
}

void FilePath::close(GPStream* s)
{
    GP_Stream_Destroy(s);
}
