/******************************************************************
 Copyright 2015, Jiang Xiao-tang
 
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

#include "utils/GPStringHelper.h"
#include "core/GPStreamFactory.h"
#include "GPStreamADF.h"
#include <sstream>
static std::string _constructNodeName(void* p)
{
    std::ostringstream os;
    os << "Point_"<<p;
    return os.str();
}
bool GPStreamADF::SP::vReceive(CONTENT con, const Point* source)
{
    GPASSERT(NULL==source);
    GPASSERT(NULL!=con.get());
    mOutputs[0]->vReceive(con, this);
    return true;
}

bool GPStreamADF::CP::vReceive(CONTENT c, const Point* source)
{
    for (int i=0; i<mInputs.size(); ++i)
    {
        if (mInputs[i] == source)
        {
            if (mPoint->receive(c, i))
            {
                auto c = mPoint->compute();
                GPASSERT(c.size() == mOutputs.size());
                for (int i=0; i<c.size(); ++i)
                {
                    mOutputs[i]->vReceive(c[i], this);
                }
            }
            return true;
        }
    }
    GPASSERT(0);
    return false;
}

GPPtr<GPTreeNode> GPStreamADF::CP::dump() const
{
    GPPtr<GPTreeNode> root = new GPTreeNode(_constructNodeName((void*)this), mPoint->get()->name);
    std::ostringstream inputs_str;
    for (auto input : mInputs)
    {
        inputs_str << _constructNodeName((void*)input)<<" ";
    }
    GPPtr<GPTreeNode> inputs = new GPTreeNode("Input", inputs_str.str());
    root->addChild(inputs);
    std::ostringstream outputs_str;
    for (auto output : mOutputs)
    {
        outputs_str << _constructNodeName((void*)(output.get())) << " ";
    }
    GPPtr<GPTreeNode> outputs = new GPTreeNode("Output", outputs_str.str());
    root->addChild(outputs);
    return root;
}


GPStreamADF::DP::DP(const IStatusType* t)
{
    mType = t;
    mContents = NULL;
}


bool GPStreamADF::DP::vReceive(CONTENT c, const Point* source)
{
    GPASSERT(1 == mInputs.size());
    mContents = c;
    return false;
}

void GPStreamADF::DP::reset()
{
    mContents = NULL;
}


GPStreamADF::GPStreamADF(const GPFunctionTree* root)
{
    auto rootfunc = root->function();
    GPASSERT(NULL!=rootfunc);
    auto lists = root->display();
    std::map<const GPAbstractPoint*, GPPtr<Point>> maplists;
    /*Create All Function*/
    for (auto p : lists)
    {
        auto pp = (GPFunctionTree*)p;
        if (NULL == pp->function())
        {
            GPPtr<Point> cp = new SP(NULL);
            mSources.push_back(cp);
            mInputPos.push_back(pp->inputNumber());
            maplists.insert(std::make_pair(p, cp));
        }
        else
        {
            GPPtr<Point> cp = new CP(new GPComputePoint(pp->function()));
            mFunctions.push_back((CP*)(cp.get()));
            maplists.insert(std::make_pair(p, cp));
        }
    }
    /*Dest*/
    auto rootcp = maplists.find(root)->second;
    for (int i=0; i<rootfunc->outputType.size(); ++i)
    {
        GPPtr<Point> dst = (new DP(rootfunc->outputType[i]));
        dst->connectInput(rootcp.get());
        rootcp->connectOutput(dst);
        mDest.push_back(dst);
    }
    /*Connect*/
    for (auto p : lists)
    {
        auto PP = maplists.find(p)->second;
        auto func = ((GPFunctionTree*)p)->function();
        size_t n = p->getChildrenNumber();
        GPASSERT(!(NULL!=func && n==0));
        for (int i=0; i<n; ++i)
        {
            auto pc = p->getChild(i);
            auto PC = maplists.find(pc)->second;
            PP->connectInput(PC.get());
            PC->connectOutput(PP);
            if (pc->getChildrenNumber() == 0)
            {
                SP* s = (SP*)PC.get();
                s->setType(func->inputType[i]);
            }
        }
    }
}


GPStreamADF::GPStreamADF(const GPTreeNode* n, const GPFunctionDataBase* base)
{
    auto children = n->getChildren();
    GPASSERT(3 == children.size());
    mSources.clear();
    /*Source*/
    std::map<std::string, Point*> sp_map;
    {
        auto source = children[0];
        GPASSERT(source->name() == "Source");
        for (auto types : source->getChildren())
        {
            auto source_name = types->name();
            auto type_name = types->attr();
            auto spoint = new SP(base->vQueryType(type_name));
            mSources.push_back(spoint);
            sp_map.insert(std::make_pair(source_name, spoint));
        }
        for (int i=0; i<mSources.size(); ++i)
        {
            mInputPos.push_back(i);
        }
    }
    std::map<std::string, GPPtr<Point>> cp_map;
    /*ComputePoints*/
    {
        auto computepoints_info = children[1];
        GPASSERT(computepoints_info->name() == "ComputePoint");
        /*Collect all points firstly*/
        for (auto compute_info : computepoints_info->getChildren())
        {
            auto function = base->vQueryFunction(GPStringHelper::cleanString(compute_info->attr()));
            if (NULL == function)
            {
                FUNC_PRINT_ALL(compute_info->attr().c_str(), s);
            }
            GPASSERT(NULL!=function);
            auto node = new CP(new GPComputePoint(function));
            mFunctions.push_back(node);
            cp_map.insert(std::make_pair(compute_info->name(), node));
        }
        
        /*Now Connect*/
        for (auto compute_info : computepoints_info->getChildren())
        {
            auto function = base->vQueryFunction(GPStringHelper::cleanString(compute_info->attr()));
            auto this_node = cp_map.find(compute_info->name())->second;
            for (auto info : compute_info->getChildren())
            {
                if (info->name() == "Input")
                {
                    auto node_names = GPStringHelper::divideString(info->attr());
                    for (auto name : node_names)
                    {
                        auto s_iter = sp_map.find(name);
                        if (s_iter == sp_map.end())
                        {
                            continue;
                        }
                        s_iter->second->connectOutput(this_node);
                        this_node->connectInput(s_iter->second);
                    }
                }
                if (info->name() == "Output")
                {
                    auto node_names = GPStringHelper::divideString(info->attr());
                    GPASSERT(function->outputType.size() == node_names.size());
                    auto n = function->outputType.size();
                    for (size_t i=0; i<n; ++i)
                    {
                        auto name = node_names[i];
                        auto cp_iter = cp_map.find(name);
                        if (cp_iter!=cp_map.end())
                        {
                            auto output = cp_iter->second;
                            this_node->connectOutput(output);
                            output->connectInput(this_node.get());
                            continue;
                        }
                        auto dst = new DP(function->outputType[i]);
                        mDest.push_back(dst);
                        dst->connectInput(this_node.get());
                        this_node->connectOutput(dst);
                        dst->addRef();
                    }
                }
            }
        }
    }
    /*Load Status*/
    {
        auto status_tn = children[2];
        GPASSERT(status_tn->name() == "Status");
        auto status_values = GPStringHelper::divideString(status_tn->attr());
        if (status_values.size() > 0)
        {
            GPASSERT(status_values.size() == this->vMap(NULL));
            GPPtr<GPParameter> para = new GPParameter((int)(status_values.size()));
            for (int i=0; i<status_values.size(); ++i)
            {
                std::istringstream number(status_values[i]);
                number >> para->attach()[i];
            }
            this->vMap(para.get());
        }
        
    }
}
GPStreamADF::~GPStreamADF()
{
    
}

GPContents* GPStreamADF::vRun(GPContents* inputs)
{
    GPASSERT(NULL!=inputs);
    GPASSERT(inputs->size() == mSources.size());
    std::vector<CONTENT> inputsWrap;
    for (int i=0; i<inputs->size(); ++i)
    {
        auto pos = mInputPos[i];
        GPASSERT(pos < inputs->size());
        inputsWrap.push_back(new GPComputePoint::ContentWrap(inputs->getContent(pos).content, inputs->getContent(pos).type));
    }
    for (int i=0; i<inputsWrap.size(); ++i)
    {
        mSources[i]->vReceive(inputsWrap[i], NULL);
        inputsWrap[i]->releaseForFree();
    }
    bool res_valid = true;
    for (auto dv : mDest)
    {
        DP* d = (DP*)dv.get();
        auto c_unit = d->get();
        if (NULL == c_unit.get())
        {
            res_valid = false;
            break;
        }
    }
    if (!res_valid)
    {
        return NULL;
    }
    auto res = new GPContents;
    for (auto dv : mDest)
    {
        DP* d = (DP*)dv.get();
        auto unit = d->get();
        res->push(unit->getContent(), unit->getType());
        unit->releaseForFree();
    }
    return res;
}

GPTreeNode* GPStreamADF::vSave() const
{
    GPTreeNode* root = new GPTreeNode("GPStreamADF", "");
    GPPtr<GPTreeNode> status_node = new GPTreeNode("Status", "");
    GPPtr<GPTreeNode> cp_node = new GPTreeNode("ComputePoint","");
    GPPtr<GPTreeNode> source_node = new GPTreeNode("Source", "");
    root->addChild(source_node);
    root->addChild(cp_node);
    root->addChild(status_node);
    
    for (auto sv : mSources)
    {
        auto s = (SP*)sv.get();
        source_node->addChild(_constructNodeName(s), s->type()->name());
    }
    for (auto c : mFunctions)
    {
        cp_node->addChild(c->dump());
    }
    return root;
}
IGPAutoDefFunction* GPStreamADF::vCopy() const
{
    return NULL;
}
int GPStreamADF::vMapStructure(GPParameter* para, bool& changed)
{
    changed = false;
    return 0;
}
int GPStreamADF::vMap(GPParameter* para)
{
    int sum = 0;
    for (auto p : mFunctions)
    {
        sum += p->get()->map(NULL, 0);
    }
    if (NULL == para)
    {
        return sum;
    }
    GPASSERT(sum == para->size());
    AutoStorage<double> __p(para->size());
    double* p = __p.get();
    for (int i=0; i<sum; ++i)
    {
        p[i] = (*para)[i];
    }
    int offset = 0;
    for (auto f : mFunctions)
    {
        offset += f->get()->map(p+offset, sum-offset);
    }
    return sum;
}

