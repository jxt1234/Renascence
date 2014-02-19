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
GenerateSystem* evolutionTree::mGen = NULL;
IFitComputer* evolutionTree::mFitComputer = NULL;

/*Assum the output of gp is double and return the compute result, don't set any input*/
class DefaultFitComputer:public IFitComputer
{
    public:
        DefaultFitComputer(IRuntimeDataBase* data, statusBasic* sta)
        {
            mRun = data;
            mSta = sta;
        }
        virtual ~DefaultFitComputer(){}
        virtual double fit_comput(AbstractGP* gp)
        {
            GP_Input nullinput;
            int cur = 0;
            gp->input(nullinput, cur);
            gp->compute(mRun, mSta);
            GP_Output output = gp->output();
            double* result = (double*)(output.output[0].content);
            double res = *result;
            /*It has duty to free the output*/
            if (output.output[0].freeCallBack)
            {
                output.output[0].freeCallBack(output.output[0].content);
            }
            return res;
        }
    private:
        IRuntimeDataBase* mRun;
        statusBasic* mSta;
};

double evolutionTree::fit_comput()
{
    if (NULL!=mFitComputer)
    {
        mFit = mFitComputer->fit_comput(mTree);
    }
    else
    {
        DefaultFitComputer c(mGen, mGen);
        mFit = c.fit_comput(mTree);
    }
    return mFit;
}
double evolutionTree::get_fit() const
{
    return mFit;
}

void evolutionTree::mutate()
{
    assert(NULL!=mGen);
    mTree->mutate(mGen);
}
evolutionTree::evolutionTree(mutateTree* self)
{
    assert(NULL!=mGen);
    if (NULL==self)
    {
        self = new mutateTree;
        int cur = 0;
        mGen->initGP(self);
    }
    mTree = self;
    mFit = 0;
}
evolutionTree::evolutionTree(const evolutionTree& tree)
{
    assert(NULL!=mGen);
    AbstractGP::AbstractGPCopy copy(mGen);
    mTree = (mutateTree*)AbstractPoint::deepCopy(tree.mTree, &copy);
    mFit = tree.mFit;
}
void evolutionTree::operator=(const evolutionTree& tree)
{
    assert(NULL!=mGen);
    AbstractGP::AbstractGPCopy copy(mGen);
    mTree = (mutateTree*)AbstractPoint::deepCopy(tree.mTree, &copy);
    mFit = tree.mFit;
}

evolutionTree::~evolutionTree()
{
    assert(NULL!=mTree);
    if (NULL!=mGen) mGen->freeStatus(mTree);
    delete mTree;
}

