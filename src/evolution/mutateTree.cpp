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
#include "evolution/mutateTree.h"
#include <assert.h>
#include <stdlib.h>

#include "utils/debug.h"

using namespace std;

float mutateTree::gLargeVary = 0.1;
float mutateTree::gStatusVary = 0.4;

class mutateTree::xmlCopy:public AbstractPoint::IPointCopy
{
    public:
        xmlCopy(GenerateSystem* sys):mSys(sys){}
        virtual ~xmlCopy(){}
        virtual AbstractPoint* copy(AbstractPoint* src)
        {
            xmlTree* t = dynamic_cast<xmlTree*>(src);
            assert(NULL!=t);
            int func = mSys->getFuncId(t->func());
            int status = -1;
            vector<int> types;
            vector<string> contents;
            const vector<xmlTree::type>& ttype = t->status();
            for (int i=0; i<ttype.size(); ++i)
            {
                int _type = status_queryId(ttype[i].name);
                types.push_back(_type);
                contents.push_back(ttype[i].content);
            }
            status = status_loadSet(types, contents);
            AbstractGP* p = new AbstractGP(func,status);
            return p;
        }
    private:
        GenerateSystem* mSys;
};

mutateTree* mutateTree::loadXmlTree(xmlTree* tree, GenerateSystem* mGen)
{
    mutateTree::xmlCopy copy(mGen);
    mutateTree* res = (mutateTree*)AbstractPoint::deepCopy(tree, &copy);
    return res;
}


void mutateTree::mutate(GenerateSystem* mGen)
{
    assert(NULL!=mGen);
    const int scale = 100;
    /*Little prob to totally changed*/
    int _rand = rand()%scale;
    if (_rand < gLargeVary*scale)
    {
        int cur = 0;
        replacePoint(mGen->getRandSequenceWithOutput(mFunc), cur);
        return;
    }
    else if (_rand < gStatusVary*scale)
    {
        status_varySet(mStatus);
    }
    for (int i=0; i<mChildren.size(); ++i)
    {
        mutateTree* p = (mutateTree*)(mChildren[i]);
        p->mutate(mGen);
    }
}

