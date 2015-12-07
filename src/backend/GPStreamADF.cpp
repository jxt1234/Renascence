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
                auto comp = mPoint->compute();
                GPASSERT(comp.size() == mOutputs.size());
                for (int j=0; j<mOutputs.size(); ++j)
                {
                    mOutputs[j]->vReceive(comp[j], this);
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


GPStreamADF::DP::DP(const IStatusType* t):Point(1, 0)
{
    mType = t;
    mContents = NULL;
}


bool GPStreamADF::DP::vReceive(CONTENT c, const Point* source)
{
    GPASSERT(1 == mInputs.size());
    if (NULL!=mContents.get())
    {
        GPASSERT(mContents->getType() == c->getType());
    }
    mContents = c;
    return false;
}


void GPStreamADF::DP::reset()
{
    mContents = NULL;
}

bool GPStreamADF::TP::vReceive(CONTENT c, const Point* source)
{
    GPASSERT(source == mInputs[0]);
    for (auto p : mOutputs)
    {
        p->vReceive(c, this);
    }
    return true;
}

GPPtr<GPTreeNode> GPStreamADF::TP::dump() const
{
    GPPtr<GPTreeNode> root = new GPTreeNode(_constructNodeName((void*)this), "");
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


GPStreamADF::GPStreamADF(const GPMultiLayerTree* opttree)
{
    GPASSERT(NULL!=opttree);
    auto layers = opttree->layers();
    std::map<const GPFunctionTreePoint*, GPPtr<Point>> maplists;
    GPASSERT(layers.size()>=1);
    /*Create All CP*/
    for (auto layer : layers)
    {
        for (auto iter:layer)
        {
            for (auto p : iter.second->display())
            {
                auto pp = (GPFunctionTreePoint*)p;
                if (NULL != pp->function())
                {
                    GPPtr<Point> cp = new CP(new GPComputePoint(pp->function()));
                    mFunctions.push_back((CP*)(cp.get()));
                    maplists.insert(std::make_pair(pp, cp));
                }
            }
        }
    }
    /*Connect Already Exists CP*/
    for (auto cpiter : maplists)
    {
        auto tree = cpiter.first;
        auto output = cpiter.second;
        size_t n = tree->getChildrenNumber();
        for (int i=0; i<n; ++i)
        {
            auto _p = GPCONVERT(const GPFunctionTreePoint, tree->getChild(i));
            if (maplists.find(_p)!=maplists.end())
            {
                auto input = maplists.find(_p)->second;
                input->connectOutput(output, 0);
                output->connectInput(input.get(), i);
            }
        }
    }
    /*Create All Inputs*/
    std::map<int, std::vector<std::pair<const GPFunctionTreePoint*, int>>> inputLists;
    for (auto iter : maplists)
    {
        auto p = iter.first;
        size_t n = p->getChildrenNumber();
        for (int i=0; i<n; ++i)
        {
            auto _p = GPCONVERT(const GPFunctionTreePoint, p->getChild(i));
            if (_p->inputNumber()>=0)
            {
                int pos = _p->inputNumber();
                if (inputLists.find(pos) == inputLists.end())
                {
                    std::vector<std::pair<const GPFunctionTreePoint*, int>> t;
                    inputLists.insert(std::make_pair(pos, t));
                }
                inputLists.find(pos)->second.push_back(std::make_pair(p, i));
            }
        }
    }
    /*Replace inputpos from layers*/
    for (int i=1; i<layers.size(); ++i)
    {
        auto layer = layers[i];
        for (auto iter : layer)
        {
            auto replacePos = iter.first;
            auto outputPoints = inputLists.find(replacePos)->second;
            GPPtr<Point> transform = new TP((int)outputPoints.size());
            mTPS.push_back((TP*)transform.get());
            GPPtr<Point> inputPoint = maplists.find(iter.second.get())->second;
            inputPoint->connectOutput(transform, 0);
            transform->connectInput(inputPoint.get(), 0);
            for (int j=0; j<outputPoints.size(); ++j)
            {
                GPPtr<Point> transform_node = new TP(1);
                mTPS.push_back((TP*)transform_node.get());
                transform->connectOutput(transform_node, j);
                transform_node->connectInput(transform.get(), 0);
                auto o_tree = outputPoints[j];
                auto o = maplists.find(o_tree.first)->second;
                o->connectInput(transform_node.get(), o_tree.second);
                transform_node->connectOutput(o, 0);
            }
            inputLists.erase(replacePos);
        }
    }
    /*Create SP*/
    GPASSERT(inputLists.size()>0);
    for (auto iter : inputLists)
    {
        for (auto v : iter.second)
        {
            auto tree = v.first;
            auto pos = v.second;
            auto point = maplists.find(tree)->second;
            GPPtr<Point> source_point = new SP(tree->function()->inputType[pos]);
            source_point->connectOutput(point, 0);
            point->connectInput(source_point.get(), pos);
            mSources.push_back(source_point);
            mInputPos.push_back(iter.first);
        }
    }
    
    /*Create DP*/
    auto firstLayer = opttree->layers()[0];
    for (auto iter : firstLayer)
    {
        GPASSERT(iter.first < 0);
        GPPtr<Point> input = maplists.find(iter.second.get())->second;
        for (size_t i=0; i<iter.second->function()->outputType.size(); ++i)
        {
            GPPtr<Point> dst = new DP(iter.second->function()->outputType[i]);
            mDest.push_back(dst);
            dst->connectInput(input.get(), 0);
            input->connectOutput(dst, 0);
        }
    }
}

GPStreamADF::GPStreamADF(const GPFunctionTree* tree)
{
    auto root = tree->root();
    auto rootfunc = root->function();
    GPASSERT(NULL!=rootfunc);
    auto lists = root->display();
    std::map<const GPAbstractPoint*, GPPtr<Point>> maplists;
    /*Create All Function*/
    for (auto p : lists)
    {
        auto pp = (GPFunctionTreePoint*)p;
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
        dst->connectInput(rootcp.get(), 0);
        rootcp->connectOutput(dst, i);
        mDest.push_back(dst);
    }
    /*Connect*/
    for (auto p : lists)
    {
        auto PP = maplists.find(p)->second;
        auto func = ((GPFunctionTreePoint*)p)->function();
        size_t n = p->getChildrenNumber();
        GPASSERT(!(NULL!=func && n==0));
        for (int i=0; i<n; ++i)
        {
            auto pc = p->getChild(i);
            auto PC = maplists.find(pc)->second;
            PP->connectInput(PC.get(), i);
            PC->connectOutput(PP, 0);
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
                    for (int i=0; i<node_names.size(); ++i)
                    {
                        auto name = node_names[i];
                        auto s_iter = sp_map.find(name);
                        if (s_iter != sp_map.end())
                        {
                            s_iter->second->connectOutput(this_node, 0);
                            this_node->connectInput(s_iter->second, i);
                            continue;
                        }
                        auto c_iter = cp_map.find(name);
                        GPASSERT(c_iter!=cp_map.end());
                        this_node->connectInput(c_iter->second.get(), i);
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
                            this_node->connectOutput(output, 0);
                            continue;
                        }
                        auto dst = new DP(function->outputType[i]);
                        mDest.push_back(dst);
                        dst->connectInput(this_node.get(), 0);
                        this_node->connectOutput(dst, 0);
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
    for (int i=0; i<inputs->size(); ++i)
    {
        //FUNC_PRINT_ALL(inputs->getContent(mInputPos[i]).content, p);
    }
    for (int i=0; i<mInputPos.size(); ++i)
    {
        auto pos = mInputPos[i];
        GPASSERT(pos < inputs->size());
        //FUNC_PRINT_ALL(inputs->getContent(mInputPos[i]).content, p);
        if (NULL!=inputs->getContent(pos).content)
        {
            CONTENT c = new GPComputePoint::ContentWrap(inputs->getContent(pos).content, inputs->getContent(pos).type);
            mSources[i]->vReceive(c, NULL);
            c->releaseForFree();
        }
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
    GPPtr<GPTreeNode> tp_node = new GPTreeNode("Transform", "");
    GPPtr<GPTreeNode> dp_node = new GPTreeNode("Dest", "");
    root->addChild(source_node);
    root->addChild(cp_node);
    root->addChild(status_node);
    root->addChild(tp_node);
    root->addChild(dp_node);
    
    for (auto sv : mSources)
    {
        auto s = (SP*)sv.get();
        source_node->addChild(_constructNodeName(s), s->type()->name());
    }
    for (auto c : mFunctions)
    {
        cp_node->addChild(c->dump());
    }
    for (auto c : mTPS)
    {
        tp_node->addChild(c->dump());
    }
    for (auto dv : mDest)
    {
        dp_node->addChild(_constructNodeName(dv.get()), "");
    }
    return root;
}
IGPAutoDefFunction* GPStreamADF::vCopy() const
{
    return NULL;
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

