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
#include "core/GP_XmlString.h"
using namespace std;
const string GP_XmlString::node("Node");
const string GP_XmlString::func("function");
const string GP_XmlString::status("status");
const string GP_XmlString::lib("libName");
const string GP_XmlString::children("childern");
const string GP_XmlString::result("result");
const string GP_XmlString::file("File:");
const string GP_XmlString::inputs("InputNodes:");
const string GP_XmlString::outputs("OutputNodes:");
void GP_XmlString::warpStart(const std::string& head, ostream& os)
{
    os << "<"<<head<<">/n";
}
void GP_XmlString::warpEnd(const std::string& head, ostream& os)
{
    os << "</"<<head<<">/n";
}
