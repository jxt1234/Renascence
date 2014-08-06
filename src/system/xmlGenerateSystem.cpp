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
#include "system/xmlGenerateSystem.h"
#include "system/mutateTree.h"
#include "system/system_lib.h"
#include "utils/debug.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "xml/xmlTree.h"

#include "utils/GP_Clock.h"

using namespace std;

class xmlCopy:public AbstractPoint::IPointCopy
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
                int _type = mSys->queryType(ttype[i].name);
                types.push_back(_type);
                contents.push_back(ttype[i].content);
            }
            /*Currently Only support one Status*/
            if (contents.empty())
            {
                status = -1;
            }
            else
            {
                const IStatusType& t = mSys->queryType(types[0]);
                istringstream in(contents[0]);
                void* c = t.load(in);
                status = mSys->allocSet(types[0], c);
            }
            AbstractGP* p = new AbstractGP(func,status);
            return p;
        }
    private:
        GenerateSystem* mSys;
};

xmlGenerateSystem::xmlGenerateSystem()
{
    mComputeSystem = new computeSystem;
}

void xmlGenerateSystem::vAddContent(std::istream& is, IFunctionTable* table)
{
    addXml(is, table, NULL);
}

void xmlGenerateSystem::addXml(const char* xmlFile, IFunctionTable* table, ostream* print)
{
    ifstream is(xmlFile);
    assert(!is.fail());
    addXml(is, table, print);
    is.close();
}
void xmlGenerateSystem::addXml(std::istream& is, IFunctionTable* table, ostream* print)
{
    xmlFunctionLoader xmlLoader;
    xmlLoader.loadStream(is);
    if (NULL==table)
    {
        table = new system_lib(xmlLoader.libName);
        mRemain.push_back(table);
    }
    mComputeSystem->loadFuncXml(xmlLoader, table, this);
    if (NULL != print)
    {
        xmlLoader.print(*print);
        mComputeSystem->print(*print);
    }
    setComputeSystem(mComputeSystem);
}

xmlGenerateSystem::~xmlGenerateSystem()
{
    clearStatusSet();
    delete mComputeSystem;
    for (int i=0; i<mRemain.size(); ++i)
    {
        delete mRemain[i];
    }
}

IGPAutoDefFunction* xmlGenerateSystem::vCreateFunctionFromIS(std::istream& is)
{
    xmlTree tree;
    tree.loadStream(is);
    xmlCopy c(this);
    AbstractGP* gp = (AbstractGP*)AbstractPoint::deepCopy(&tree, &c);
    IGPAutoDefFunction* f = this->vCreateADFFromGP(gp);
    gp->decRef();
    return f;
}

void xmlGenerateSystem::xmlPrint(std::ostream& out, AbstractGP* gp)
{
    assert(NULL!=gp);
    //vector<int> nullVector;
    //gp->save(this, this, nullVector);
    gp->xmlPrint(out, mComputeSystem, this);
}

IGPAutoDefFunction* xmlGenerateSystem::vCreateADFFromGP(AbstractGP* gp)
{
    return new xmlGPADF(this, gp);
}

IGPAutoDefFunction* xmlGenerateSystem::vCreateFromADF(IGPAutoDefFunction* src)
{
    xmlGPADF* s = dynamic_cast<xmlGPADF*>(src);
    assert(NULL!=s);
    IGPAutoDefFunction* res = new xmlGPADF(*s);
    return res;
}

xmlGPADF::xmlGPADF(xmlGenerateSystem* sys, AbstractGP* base):mSys(sys), mBase(base)
{
    if (NULL == mBase)
    {
        mBase = new AbstractGP;
    }
    else
    {
        mBase->addRef();
    }
}
xmlGPADF::xmlGPADF(const xmlGPADF& adf)
{
    mSys = adf.mSys;
    assert(NULL!=mSys);
    AbstractGP::AbstractGPCopy copy(mSys);
    mBase = (AbstractGP*)AbstractPoint::deepCopy(adf.mBase, &copy);
}
void xmlGPADF::operator=(const xmlGPADF& adf)
{
    assert(mSys == adf.mSys);
    if (mBase->count() == 1)
    {
        mSys->freeStatus(mBase);
    }
    mBase->decRef();
    AbstractGP::AbstractGPCopy copy(mSys);
    mBase = (AbstractGP*)AbstractPoint::deepCopy(adf.mBase, &copy);
}
xmlGPADF::~xmlGPADF()
{
    if (mBase->count() == 1)
    {
        mSys->freeStatus(mBase);
    }
    mBase->decRef();
}
void xmlGPADF::save(std::ostream& os)
{
    mSys->xmlPrint(os, mBase);
}
void xmlGPADF::load(std::istream& is)
{
}
void xmlGPADF::mutate()
{
    mutateTree* tree = (mutateTree*)mBase;
    tree->mutate(mSys);
}
