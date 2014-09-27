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
#include <string>
#include <vector>
#include <istream>

#define XMLAPI


class xmlReader
{
    public:
        struct package
        {
            std::string name;
            std::vector<std::string> attr;
            std::vector<package*> children;
        };
        XMLAPI const package* loadFile(const char* file);
        XMLAPI const package* loadStream(std::istream& input);
        XMLAPI void writeFile(const char* file);
        xmlReader();
        virtual ~xmlReader();
    protected:
        virtual void attributeUnflatten() {}
        void clear();
        //For child class to clear their self data
        virtual void subClear(){}
        void analysisLine(const std::string& line);
        package* loadPackage(std::istream& input);
        package* mAttributes;
    private:
        std::vector<package*> mPackage;
        std::string mString;
        package* mCurPackage;
};

class xmlReaderTest:public xmlReader
{
    public:
        XMLAPI void print();
        xmlReaderTest(){}
        virtual ~xmlReaderTest(){}
    protected:
        void printUnit(package* p);
        virtual void attributeUnflatten(){print();}
};

#endif
