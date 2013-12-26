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

#include <xml/xmlReader.h>
#include <math/AbstractPoint.h>
#include <string>
#include <vector>


class xmlTree:public xmlReader, public AbstractPoint
{
    public:
        struct type
        {
            std::string name;
            std::string content;
        };
        xmlTree(){}
        virtual ~xmlTree(){}
        inline const std::string& func(){return mFunc;}
        inline const std::vector<type>& status(){return mStatus;}
    protected:
        virtual void attributeUnflatten();
        virtual void printBefore(std::ostream& out);
        virtual void printAfter(std::ostream& out);
        void loadNode(xmlReader::package* p);
        /*Variables*/
        std::string mFunc;
        std::vector<type> mStatus;
        std::vector<type> mResult;
    private:
        void loadValues(std::vector<type>& result, xmlReader::package* p);
        void printValues(std::vector<type>& result, std::ostream& out);
};



#endif
