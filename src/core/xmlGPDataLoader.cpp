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
#include "core/xmlGPDataLoader.h"
#include <utils/debug.h>
#include <sstream>
#include <fstream>
#include "core/GP_XmlString.h"

xmlGPDataLoader::xmlGPDataLoader(const GPFunctionDataBase& sys):mSys(sys)
{
    mData = NULL;
}

xmlGPDataLoader::~xmlGPDataLoader()
{
    if (NULL!=mData)
    {
        mData->decRef();
    }
}

void xmlGPDataLoader::attributeUnflatten()
{
    GPASSERT(NULL!=mAttributes);
    mData = new GPData(mAttributes->name);
    for (int i=0; i<mAttributes->children.size(); ++i)
    {
        xmlReader::package* cur = mAttributes->children[i];
        std::string& name = cur->name;
        std::vector<std::string>& attr = cur->attr;
        if (attr.empty()) continue;
        const IStatusType& t = *(mSys.vQueryType(name));
        void* content = NULL;
        if (attr[0] == GP_XmlString::file && attr.size() >=2)//Load from file
        {
            std::ifstream is(attr[1].c_str());
            content = t.vLoad(is);
            is.close();
        }
        else
        {
            std::ostringstream os;
            int l = attr.size();
            for (int j=0; j<l-1; ++j)
            {
                os << attr[j] << " ";
            }
            if (l >= 1)
            {
                os << cur->attr[l-1];
            }
            std::istringstream is(os.str());
            content = t.vLoad(is);
        }
        mData->addData(content, t);
    }
}
