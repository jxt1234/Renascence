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
#include <sstream>

using namespace std;
void FilePath::setEnvPath(const char* str)
{
    gEnvPath = string(str);
}


string FilePath::gEnvPath("");

struct typeString
{
    int type;
    string content;
};

static typeString gMap[] = {
    {FilePath::RUNTIME, string("runtime")},
    {FilePath::COMPARE, string("compare")},
    {FilePath::INPUT, string("inputTable")},
    {FilePath::OUTPUT, string("outputTable")},
    {FilePath::AUTOMACHINE, string("autoMachine")},
    {FilePath::STANDARD, string("standard")}
};


string FilePath::file(FilePath::TYPE t)
{
    const int l = sizeof(gMap)/sizeof(typeString);
    ostringstream os;
    os << gEnvPath;
    for (int i=0; i<l; ++i)
    {
        if (gMap[i].type = t)
        {
            os << gMap[i].content;
            break;
        }
    }
    os <<".xml";
    return os.str();
}

bool FilePath::open(FilePath::TYPE t, ifstream& is)
{
    string name = file(t);
    is.open(name.c_str());
    if (is.fail())
    {
        return false;
    }
    return true;
}
