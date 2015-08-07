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
#include <list>
#include <algorithm>
#include <sstream>
#include <stdlib.h>
#include "math/FormulaTree.h"
#include "utils/debug.h"
#include "core/GP_XmlString.h"
#include "utils/GPRandom.h"
#include "xml/xmlTree.h"
#include "producer/GPTreeProducer.h"
#include "recurse_tree.h"
#include "producer/GPTreeADF.h"
using namespace std;

class xmlCopy:public AbstractPoint::IPointCopy
{
public:
     xmlCopy(const GPFunctionDataBase* sys):mSys(sys){}
     virtual ~xmlCopy(){}
     virtual AbstractPoint* copy(AbstractPoint* src)
     {
          xmlTree* t = dynamic_cast<xmlTree*>(src);
          GPASSERT(NULL!=t);
          const GPFunctionDataBase::function* f = mSys->vQueryFunction(t->func());
          GPTreeADFPoint* p = new GPTreeADFPoint(f);
          vector<const IStatusType*> types;
          const vector<xmlTree::type>& ttype = t->status();
          vector<istream*> contents;
          for (int i=0; i<ttype.size(); ++i)
          {
               const IStatusType* _type = mSys->vQueryType(ttype[i].name);
               types.push_back(_type);
               contents.push_back(new istringstream(ttype[i].content));
          }
          p->initStatus(contents);
          for (int i=0; i<contents.size(); ++i)
          {
               delete contents[i];
          }
          return p;
     }
private:
     const GPFunctionDataBase* mSys;
};

class formulaCopy:public AbstractPoint::IPointCopy
{
public:
     formulaCopy(const GPFunctionDataBase* base):mBase(base){}
     virtual ~formulaCopy(){}
     virtual AbstractPoint* copy(AbstractPoint* src)
     {
          FormulaTreePoint* point = (FormulaTreePoint*)(src);
          GPASSERT(NULL!=point);
          if (FormulaTreePoint::NUM == point->type())
          {
               return NULL;
          }
          const GPFunctionDataBase::function* f = mBase->vQueryFunctionByShortName(point->name());
          GPTreeADFPoint* p = new GPTreeADFPoint(f);
          return p;
     }
private:
     const GPFunctionDataBase* mBase;
};


IGPAutoDefFunction* GPTreeProducer::vCreateFunctionFromFormula(const std::string& formula) const
{
     FormulaTree tree;
     tree.setFormula(formula);
     formulaCopy c(mDataBase);
     GPTreeADFPoint* p = (GPTreeADFPoint*)(AbstractPoint::deepCopy(tree.root(), &c));
     return new GPTreeADF(p, this);
}


IGPAutoDefFunction* GPTreeProducer::vCreateFunctionFromName(const std::string& name) const
{
     GPASSERT(NULL!=mDataBase);
     const GPFunctionDataBase::function* f = mDataBase->vQueryFunction(name);
     class simpleADF:public IGPAutoDefFunction
     {
     public:
          simpleADF(const GPFunctionDataBase::function& f):mFunc(f)
          {
               mInputTypes = f.inputType;
               mOutputTypes = f.outputType;
          }
          ~simpleADF(){}
          
          virtual GPContents* vRun(GPContents* input)
          {
               return mFunc.basic(input);
          }
          virtual GPTreeNode* vSave() const
          {
               GPTreeNode* root = new GPTreeNode("simpleADF", "");
               root->addChild(GP_XmlString::func, mFunc.name);
               return root;
          }
          virtual IGPAutoDefFunction* vCopy() const
          {
               IGPAutoDefFunction* r = new simpleADF(mFunc);
               return r;
          }
          virtual int vMap(GPPtr<GPParameter> para)
          {
               return 0;
          }
          virtual void vMutate()
          {
               
          }
          
     private:
          const GPFunctionDataBase::function& mFunc;
     };
     IGPAutoDefFunction* result = new simpleADF(*f);
     return result;
}


GPTreeProducer::GPTreeProducer(const GPFunctionDataBase* comsys):GPProducer("GPTreeProducer")
{
     mDataBase = comsys;
     _init();
}


/*FIXME Currently, we assume random be false and inputRepeat be true, just return the first short tree by algorithm*/
IGPAutoDefFunction* GPTreeProducer::vCreateFunction(const std::vector<const IStatusType*>& outputType, const std::vector<const IStatusType*>& inputType, bool inputRepeat, bool random) const
{
     GPASSERT(NULL!=mDataBase);
     if (!inputRepeat || random)
     {
          auto allfunctions = vCreateAllFunction(outputType,inputType, inputRepeat);
          if (allfunctions.empty())
          {
               return NULL;
          }
          size_t cur = 0;
          if (random)
          {
               cur = GPRandom::mid(0, allfunctions.size());
          }
          for (size_t i=0; i<allfunctions.size(); ++i)
          {
               if (cur != i)
               {
                    allfunctions[i]->decRef();
               }
          }
          return allfunctions[cur];
     }
     /*TODO if inputType and outputType is the same as last one, return the cached one*/
     /*Find all available output function*/
     vector<vector<int> > warpOutput;
     _findMatchedFuncton(warpOutput, outputType, inputType);
     if (warpOutput.empty())
     {
          return NULL;
     }
     vector<int> avail(1,warpOutput.size()-1);
     /*Get All sequence*/
     computePoint* start = new computePoint(warpOutput, avail, inputType, mDataBase);
     computeSearchTree tree(start);
     vector<int> queue = tree.searchOne();
     if (queue.empty())
     {
          return NULL;
     }
     GPTreeADFPoint* root = new GPTreeADFPoint;
     root->replacePoint(queue, mDataBase);
     auto gp = new GPTreeADF(root, this);
     return gp;
}

void GPTreeProducer::searchAllSequences(std::vector<std::vector<int> >& res, const std::vector<const IStatusType*>& outputType, const std::vector<const IStatusType*>& inputType, bool inputRepeat) const
{
     /*TODO if the inputType and outputType is the same as the last one, return the cached one*/
     /*Find all available output function*/
     vector<vector<int> > warpOutput;
     _findMatchedFuncton(warpOutput, outputType, inputType);
     if (warpOutput.empty())
     {
          return;
     }
     vector<int> avail;
     for (int i=0; i<warpOutput.size(); ++i)
     {
          avail.push_back(i);
     }
     /*Get All sequence*/
     computePoint* start = new computePoint(warpOutput, avail, inputType, mDataBase);
     computeSearchTree tree(start);
     res = tree.searchAll();
}

static bool inOrder(const std::vector<size_t>& order)
{
     for (int i=0; i<order.size(); ++i)
     {
          if (i!=order[i])
          {
               return false;
          }
     }
     return true;
}

static bool makeOrder(std::vector<const IStatusType*> inputType, const std::vector<const IStatusType*>& realinputType, std::vector<size_t>& order/*output*/)
{
     bool result = true;
     order.clear();
     for (int i=0; i<realinputType.size(); ++i)
     {
          auto t = realinputType[i];
          auto pos = std::find(inputType.begin(), inputType.end(), t);
          if (pos == inputType.end())
          {
               result = false;
               break;
          }
          *pos = NULL;//Remove the used input
          auto id = pos - inputType.begin();
          order.push_back(id);
     }
     return result;
}


std::vector<IGPAutoDefFunction*> GPTreeProducer::vCreateAllFunction(const std::vector<const IStatusType*>& outputType, const std::vector<const IStatusType*>& inputType, bool inputRepeat) const
{
     GPASSERT(NULL!=mDataBase);
     vector<IGPAutoDefFunction*> res;
     vector<vector<int> >queue;
     searchAllSequences(queue, outputType, inputType, inputRepeat);
     for (int i=0; i<queue.size(); ++i)
     {
          GPTreeADFPoint* root = new GPTreeADFPoint;
          root->replacePoint(queue[i], mDataBase);
          auto gp = new GPTreeADF(root, this);
          if (!inputRepeat)
          {
               auto realinputtype = gp->getInputTypes();
               std::vector<size_t> order;
               bool ok = makeOrder(inputType, realinputtype, order);
               if (ok)
               {
                    if (!inOrder(order))
                    {
                         auto agp = IGPAutoDefFunction::makeAdaptorFunction(gp, order, inputType);
                         res.push_back(agp);
                    }
                    else
                    {
                         gp->addRef();
                         res.push_back(gp);
                    }
               }
               gp->decRef();
          }
          else
          {
               res.push_back(gp);
          }
     }
     return res;
}


void GPTreeProducer::_findMatchedFuncton(std::vector<std::vector<int> >& warpOutput, const std::vector<const IStatusType*>& outputType, const std::vector<const IStatusType*>& inputType) const
{
     for (int i=0; i < mDataBase->size(); ++i)
     {
          const GPFunctionDataBase::function* f = mDataBase->vQueryFunctionById(i);
          const vector<const IStatusType*>& out = f->outputType;
          bool match = true;
          for (int j=0; j<outputType.size(); ++j)
          {
               if (find(out.begin(), out.end(), outputType[j]) == out.end())
               {
                    match = false;
                    break;
               }
          }
          for (auto t:f->inputType)
          {
               if (find(inputType.begin(), inputType.end(), t) == inputType.end())
               {
                    match = false;
                    break;
               }
          }
          if (match)
          {
               vector<int> output;
               output.push_back(i);
               warpOutput.push_back(output);
          }
     }
}

void GPTreeProducer::_init()
{
     mLargeVary = 0.1;
     mStatusVary = 0.4;
}
IGPAutoDefFunction* GPTreeProducer::vCreateFunctionFromNode(const GPTreeNode* node) const
{
     xmlCopy c(mDataBase);
     xmlTree tree(node);
     GPTreeADFPoint* p = (GPTreeADFPoint*)AbstractPoint::deepCopy(&tree, &c);
     return new GPTreeADF(p, this);
}


