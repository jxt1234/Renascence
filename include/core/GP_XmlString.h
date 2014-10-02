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
#ifndef CORE_GP_XMLSTRING_H
#define CORE_GP_XMLSTRING_H
#include <string>
#include <ostream>
class GP_XmlString
{
    public:
        static const std::string node;
        static const std::string func;
        static const std::string lib;
        static const std::string status;
        static const std::string children;
        static const std::string result;
        static const std::string inputs;
        static const std::string outputs;
        static const std::string type;

        static const std::string file;

        static void warpStart(const std::string& head, std::ostream& os);
        static void warpEnd(const std::string& head, std::ostream& os);
};

#endif
