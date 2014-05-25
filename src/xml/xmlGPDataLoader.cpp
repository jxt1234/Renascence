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
#include "xml/xmlGPDataLoader.h"
#include <assert.h>
#include <sstream>

xmlGPDataLoader::xmlGPDataLoader(statusBasic& sys):mSys(sys)
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
    assert(NULL!=mAttributes);
    mData = new GPData(mAttributes->name);
    for (int i=0; i<mAttributes->children.size(); ++i)
    {
        xmlReader::package* cur = mAttributes->children[i];
        std::string& name = cur->name;
        int id = mSys.queryType(name);
        const IStatusType& t = mSys.queryType(id);
        std::ostringstream os;
        for (int j=0; j<cur->attr.size(); ++j)
        {
            os << cur->attr[j] << " ";
        }
        std::istringstream is(os.str());
        void* content = t.load(is);
        mData->addData(content, t);
    }
}
