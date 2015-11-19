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
#include <list>
#include <sstream>
#include <map>
#include "core/GP_XmlString.h"
#include "backend/GPGraphicADF.h"
#include "utils/GPStringHelper.h"

/*For debug*/
static void valid(const std::string& name)
{
    GPASSERT(name.find(GP_XmlString::node) != std::string::npos);
}


typedef GPGraphicADF::Point _POINT;

int _POINT::gNumber = 0;

int _POINT::numberOfInstance()
{
    return gNumber;
}

_POINT::Point(const GPFunctionDataBase::function* f, const IStatusType* type):mFunc(f), mType(type)
{
    /*For input and output point, we needn't init status*/
    gNumber++;//FIXME For debug
}

_POINT::~Point()
{
    mOutputs.clear();
    mInputs.clear();
    mStatus.clear();
    gNumber--;//FIXME For debug
}

void _POINT::initStatus(const std::vector<std::istream*>& statusInput)
{
    if (NULL == mFunc) return;
    GPASSERT(statusInput.size() == mStatus.size());
    const std::vector<const IStatusType*>& statustype = mFunc->statusType;
    GPASSERT(statusInput.size() == statustype.size());
    for (int i=0; i<mStatus.size(); ++i)
    {
        if (NULL==statusInput[i]) continue;
        std::istream& is = *(statusInput[i]);
        GPStatusContent* s= mStatus[i].get();
        AutoStorage<double> _v(s->size());
        double* v = _v.get();
        for (int j=0; j<s->size(); ++j)
        {
            is >> v[j];
        }
        s->setValue(v, s->size());
    }
}

void _POINT::_clean()
{
    for (auto i = mInputs.begin(); i!=mInputs.end(); i++)
    {
        i->second = NULL;//The Unit will be unreference
    }
}

void _POINT::_reset()
{
    _clean();
    for (auto i=mOutputs.begin(); i!=mOutputs.end(); i++)
    {
        (*i)->_reset();
    }
}

GPGraphicADF::UNIT _POINT::collect()
{
    /*Multi output is not allowed*/
    GPASSERT(mInputs.size() == 1);
    Unit* u = ((mInputs.begin())->second).get();
    UNIT result = u->warp();
    _clean();
    return result;
}

bool _POINT::_flow(bool clean)
{
    if (NULL != mFunc)
    {
        GPContents inputs;
        /*Inputs from user or function*/
        for (auto i = mInputs.begin(); i!=mInputs.end(); i++)
        {
            inputs.push(i->second->warp());
        }
        /*Inputs from self status*/
        for (int i=0; i<mStatus.size(); ++i)
        {
            inputs.push((mStatus.at(i))->content(), (mStatus.at(i))->type());
        }

        auto out = mFunc->basic(&inputs);
        GPASSERT(out->size() == mOutputs.size());

        /*Send output*/
        for (int i=0; i<out->size(); ++i)
        {
            GPPtr<Unit> u = new Unit(out->contents[i]);
            (mOutputs.at(i))->receive(u, this);
        }
        if (clean)
        {
            _clean();
        }
        delete out;
    }
    for (auto i = mOutputs.begin(); i!=mOutputs.end(); i++)
    {
        if ((*i)->ready())
        {
            (*i)->_flow(clean);
        }
    }
    return true;
}
bool _POINT::ready() const
{
    bool result = true;
    for (auto i = mInputs.begin(); i!=mInputs.end(); i++)
    {
        if (NULL == (i->second).get())
        {
            result = false;
            break;
        }
    }
    return result;
}

void _POINT::connectInput(Point* in)
{
    mInputs.push_back(std::make_pair(in, GPPtr<Unit>(NULL)));
}

void _POINT::connectOutput(GPPtr<Point> out)
{
    mOutputs.push_back(out);
}

void _POINT::receive(GPPtr<GPGraphicADF::Unit> u, _POINT* source)
{
    /*For input points can only has one output, it just pass the data to midpoints*/
    if (NULL == mFunc && mInputs.empty())
    {
        GPASSERT(mOutputs.size() == 1);
        (*(mOutputs.begin()))->receive(u, this);
        return;
    }
    /*Mid points should has position for the data, otherwise the connection is wrong*/
    bool find = false;
    for (auto i=mInputs.begin(); i!=mInputs.end(); i++)
    {
        if (i->first == source)
        {
            i->second = u;
            find = true;
            break;
        }
    }
    GPASSERT(true == find);
}

GPContents* GPGraphicADF::vRun(GPContents* inputs)
{
    GPASSERT(inputs->size() == mInputs.size());
    for (int i=0; i<inputs->size(); ++i)
    {
        GPPtr<Unit> u = new Unit(inputs->contents[i]);
        (mInputs.at(i))->receive(u, NULL);
    }
    for (int i=0; i<mInputs.size(); ++i)
    {
        (mInputs.at(i))->flow(true);
    }
    GPContents* result = new GPContents;
    for (int i=0; i<mOutputs.size(); ++i)
    {
        UNIT r = (mOutputs.at(i))->collect();
        result->push(r);
    }
    return result;
}
IGPAutoDefFunction* GPGraphicADF::vCopy() const
{
    /*TODO*/
    return NULL;
}

void GPGraphicADF::_findAllPoints(std::set<Point*>& allPoints) const
{
    GPASSERT(allPoints.empty());
    std::list<Point*> cachePoints;
    for (int i=0; i<mInputs.size(); ++i)
    {
        cachePoints.push_back(mInputs.at(i).get());
    }
    while(!cachePoints.empty())
    {
        Point* p = cachePoints.front();
        if (allPoints.find(p)==allPoints.end())
        {
            allPoints.insert(p);
        }
        std::vector<GPPtr<Point> >& outputs = p->mOutputs;
        for (int i=0; i<outputs.size(); ++i)
        {
            cachePoints.push_back(outputs.at(i).get());
        }
        cachePoints.pop_front();
    }
}
static std::string combinenode(_POINT* p)
{
    std::ostringstream os;
    os << GP_XmlString::node << "_"<<p;
    return os.str();
}

GPTreeNode* GPGraphicADF::vSave() const
{
    /*Find all points*/
    std::set<Point*> allPoints;
    _findAllPoints(allPoints);
    GPASSERT(!allPoints.empty());
    /*Print Points, Don't care about the Order*/
    GPTreeNode* root = new GPTreeNode("GPGraphicADF", "");
    std::set<Point*>::iterator iter = allPoints.begin();
    for (;iter!=allPoints.end(); iter++)
    {
        Point* cur = *iter;
        GPPtr<GPTreeNode> node = new GPTreeNode(combinenode(cur), "");
        root->addChild(node);
        if (NULL!=cur->mFunc)
        {
            node->addChild(GP_XmlString::func, cur->mFunc->name);
        }
        else
        {
            if (NULL!=cur->mType)
            {
                node->addChild(GP_XmlString::type, cur->mType->name());
            }
            else
            {
                node->addChild(GP_XmlString::type, "NULL");
            }
        }
        GPPtr<GPTreeNode> inputsnode = new GPTreeNode(GP_XmlString::inputs, "");
        node->addChild(inputsnode);
        for (auto i=cur->mInputs.begin(); i!=cur->mInputs.end();i++)
        {
            inputsnode->addChild(combinenode(i->first), "");
        }
        GPPtr<GPTreeNode> outputnodes = new GPTreeNode(GP_XmlString::outputs, "");
        node->addChild(outputnodes);
        for (auto i=cur->mOutputs.begin(); i!=cur->mOutputs.end();i++)
        {
            outputnodes->addChild(combinenode(i->get()), "");
        }
        GPPtr<GPTreeNode> statusnode = new GPTreeNode(GP_XmlString::status, "");
        node->addChild(statusnode);
        for (auto i=cur->mStatus.begin(); i!=cur->mStatus.end(); i++)
        {
            const IStatusType* s = (*i)->type();
            std::ostringstream os;
            (*i)->print(os);
            GPPtr<GPTreeNode> piecestatus = new GPTreeNode(s->name(), os.str());
            statusnode->addChild(piecestatus);
        }
    }
    return root;
}

void GPGraphicADF::_loadMain(const GPTreeNode* root, std::map<std::string, Point*>& allPoints, const GPFunctionDataBase* base) const
{
    auto children = root->getChildren();
    for (auto iter : children)
    {
        const GPTreeNode* node = iter.get();
        valid(node->name());
        GPASSERT(allPoints.find(node->name())==allPoints.end());//FIXME Print Error instead of GPASSERT
        auto funcattr = node->getChildren();
        GPASSERT(funcattr.size()>1);
        auto first = funcattr[0];
        Point* p = NULL;
        if (first->name() == GP_XmlString::func)
        {
            const GPFunctionDataBase::function* _f = base->vQueryFunction(first->attr());
            GPASSERT(NULL!=_f);
            p = new Point(_f, NULL);
        }
        else if (first->name() == GP_XmlString::type)
        {
            const IStatusType* type = base->vQueryType(first->attr());
            p = new Point(NULL, type);
        }
        GPASSERT(NULL!=p);//FIXME Print error instead of GPASSERT
        allPoints.insert(std::make_pair(node->name(), p));
    }
}

void GPGraphicADF::_loadStatus(const GPTreeNode* attach, const GPFunctionDataBase* base, Point* currentPoint) const
{
    auto n = (currentPoint->mFunc->statusType).size();
    auto statuses = attach->getChildren();
    GPASSERT(n == statuses.size());
    std::vector<std::istream*> statusContents;
    statusContents.reserve(n);
    for (auto iter_status : statuses)
    {
        const GPTreeNode* s = iter_status.get();
        statusContents.push_back(new std::istringstream(s->attr()));
    }
    currentPoint->initStatus(statusContents);
    for (int i=0; i<statusContents.size(); ++i)
    {
        if (NULL!=statusContents.at(i))
        {
            delete statusContents.at(i);
        }
    }
}
void GPGraphicADF::_loadInputs(const GPTreeNode* attach, const std::map<std::string, Point*>& allPoints, Point* currentPoint)
{
    auto childrens = GPStringHelper::divideString(attach->attr());
    for (int i=0; i<childrens.size(); ++i)
    {
        Point* p = (allPoints.find(childrens[i]))->second;
        currentPoint->connectInput(p);
    }
    //Has inputs points but no function to handle it, mean it's output point
    if (NULL == currentPoint->mFunc && !childrens.empty())
    {
        GPPtr<Point> warp = currentPoint;
        mOutputs.push_back(warp);
        currentPoint->addRef();
    }
}
void GPGraphicADF::_loadOutputs(const GPTreeNode* attach, const std::map<std::string, Point*>& allPoints, Point* currentPoint)
{
    auto childrens = GPStringHelper::divideString(attach->attr());
    for (int i=0; i<childrens.size(); ++i)
    {
        Point* p = (allPoints.find(childrens[i]))->second;
        p->addRef();
        /*The GPPtr will auto dec reference for p, so add reference before construct GPPtr*/
        GPPtr<Point> warp = p;
        currentPoint->connectOutput(warp);
    }
    if (NULL == currentPoint->mFunc && !childrens.empty())
    {
        //Has Outputs points but no function to compute to Output, mean it's input point
        GPPtr<Point> warp = currentPoint;
        mInputs.push_back(warp);
        currentPoint->addRef();
    }
}

GPGraphicADF::GPGraphicADF(const GPTreeNode* root, const GPFunctionDataBase* base)
{
    /*Construct all Node firstly*/
    std::map<std::string, Point*> allPoints;
    _loadMain(root, allPoints, base);
    /*Connect all points and Construct status*/
    for (auto iter : root->getChildren())
    {
        const GPTreeNode* node = iter.get();
        Point* currentPoint = (allPoints.find(node->name()))->second;
        for (auto iter_node : node->getChildren())
        {
            auto attach = iter_node.get();
            if (attach->name() == GP_XmlString::status)
            {
                _loadStatus(attach, base, currentPoint);
            }
            else if(attach->name() == GP_XmlString::inputs)
            {
                _loadInputs(attach, allPoints, currentPoint);
            }
            else if(attach->name() == GP_XmlString::outputs)
            {
                _loadOutputs(attach, allPoints, currentPoint);
            }
        }
    }
    //UnRef points newed by _loadMain
    {
        std::map<std::string, Point*>::iterator iter; 
        for (iter=allPoints.begin(); iter!=allPoints.end(); iter++)
        {
            iter->second->decRef();
        }
    }
    GPASSERT(!mInputs.empty());
    GPASSERT(!mOutputs.empty());
    /*TODO Compute the input and output*/
}
GPGraphicADF::~GPGraphicADF()
{
}

int GPGraphicADF::vMap(GPParameter* p)
{
    return 0;
}
