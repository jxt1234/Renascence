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
#include "evolution/evolutionTree.h"
#include <assert.h>
#include <stdlib.h>

#include "utils/debug.h"

using namespace std;

float evolutionTree::gLargeVary = 0.1;
float evolutionTree::gStatusVary = 0.4;
GenerateSystem* evolutionTree::mGen = NULL;

class evolutionTree::xmlCopy:public AbstractPoint::IPointCopy
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

evolutionTree* evolutionTree::loadXmlTree(xmlTree* tree)
{
    evolutionTree::xmlCopy copy(mGen);
    evolutionTree* res = (evolutionTree*)AbstractPoint::deepCopy(tree, &copy);
    return res;
}

void evolutionTree::operator=(const evolutionTree& tree)
{
    AbstractGP::operator=(tree);
    mSave = NULL;
    if (tree.mSave!=NULL)
    {
        mSave = new GP_Output;
        *mSave = *(tree.mSave);
        double *fit = new double;
        (mSave->output[0]).content = fit;
        *fit = tree.get_fit();
    }
}
double evolutionTree::fit_comput()
{
    compute(mGen);
    double result = get_fit();
    return result;
}

evolutionTree::evolutionTree()
{
    assert(mGen!=NULL);
    int cur = 0;
    replacePoint(mGen->getRandSequence(), cur);
}

double evolutionTree::get_fit() const
{
    double* result;
    result = (double*)((mSave->output[0]).content);
    return *result;
}

void evolutionTree::mutate()
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
        evolutionTree* p = (evolutionTree*)(mChildren[i]);
        p->mutate();
    }
}



