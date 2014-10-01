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
#include "core/GP_XmlString.h"
#include "producer/GPGraphicADF.h"
#include "xml/xmlReader.h"
#include <map>
#include <sstream>

typedef std::vector<xmlReader::package*>::const_iterator PITER;;
/*For debug*/
static void valid(const std::string& name)
{
    assert(name.find(GP_XmlString::node) != std::string::npos);
}


typedef GPGraphicADF::Point _POINT;

int _POINT::gNumber = 0;

int _POINT::numberOfInstance()
{
    return gNumber;
}

_POINT::Point(const GPFunctionDataBase::function* f):mFunc(f)
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
    const std::vector<const IStatusType*>& status = mFunc->statusType;
    mStatus.clear();
    assert(statusInput.size() == status.size());
    for (int i=0; i<status.size(); ++i)
    {
        GPPtr<GPStatusContent> s;
        s = new GPStatusContent(status.at(i), statusInput[i]);
        mStatus.push_back(s);
    }
}

void _POINT::_clean()
{
    for (INPUT_ITER i = mInputs.begin(); i!=mInputs.end(); i++)
    {
        i->second = NULL;//The Unit will be unreference
    }
}

void _POINT::_reset()
{
    _clean();
    for (OUTPUT_ITER i=mOutputs.begin(); i!=mOutputs.end(); i++)
    {
        (*i)->_reset();
    }
}

GPGraphicADF::UNIT _POINT::collect()
{
    /*Multi output is not allowed*/
    assert(mInputs.size() == 1);
    Unit* u = ((mInputs.begin())->second).get();
    UNIT result = u->warp();
    _clean();
    return result;
}

bool _POINT::_flow(bool clean)
{
    if (NULL != mFunc)
    {
        GP_Input inputs;
        /*Inputs from user or function*/
        for (INPUT_ITER i = mInputs.begin(); i!=mInputs.end(); i++)
        {
            void* c = i->second->content();
            assert(NULL!=c);
            inputs.push_back(c);
        }
        /*Inputs from self status*/
        for (int i=0; i<mStatus.size(); ++i)
        {
            inputs.push_back((mStatus.at(i))->content());
        }

        GP_Output out = mFunc->basic(inputs);
        assert(out.size() == mOutputs.size());

        /*Send output*/
        for (int i=0; i<out.size(); ++i)
        {
            GPPtr<Unit> u = new Unit(out.output[i]);
            (mOutputs.at(i))->receive(u, this);
        }
        if (clean)
        {
            _clean();
        }
    }
    for (OUTPUT_ITER i = mOutputs.begin(); i!=mOutputs.end(); i++)
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
    for (INPUT_ITER_CONST i = mInputs.begin(); i!=mInputs.end(); i++)
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
    /*Output points shouldn't be called by this function*/
    /*For input points can only has one output, it just pass the data to midpoints*/
    if (NULL == mFunc)
    {
        assert(mInputs.empty());
        assert(mOutputs.size() == 1);
        (*(mOutputs.begin()))->receive(u, this);
    }
    /*Mid points should has position for the data, otherwise the connection is wrong*/
    bool find = false;
    for (INPUT_ITER i=mInputs.begin(); i!=mInputs.end(); i++)
    {
        if (i->first == source)
        {
            i->second = u;
            find = true;
            break;
        }
    }
    assert(true == find);
}

GP_Output GPGraphicADF::run(const GP_Input& inputs)
{
    assert(inputs.size() == mInputs.size());
    for (int i=0; i<inputs.size(); ++i)
    {
        GPPtr<Unit> u = new Unit(inputs.at(i));
        (mInputs.at(i))->receive(u, NULL);
    }
    for (int i=0; i<mInputs.size(); ++i)
    {
        (mInputs.at(i))->flow(true);
    }
    GP_Output result;
    for (int i=0; i<mOutputs.size(); ++i)
    {
        UNIT r = (mOutputs.at(i))->collect();
        result.output.push_back(r);
    }
    return result;
}
IGPAutoDefFunction* GPGraphicADF::copy() const
{
    return NULL;
}

void GPGraphicADF::_findAllPoints(std::set<Point*>& allPoints) const
{
    assert(allPoints.empty());
    std::list<Point*> cachePoints;
    for (int i=0; i<mInputs.size(); ++i)
    {
        cachePoints.push_back(mInputs.at(i).get());
    }
    while(!cachePoints.empty())
    {
        Point* p = cachePoints.front();
        if (allPoints.find(p)!=allPoints.end())
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

void GPGraphicADF::save(std::ostream& os) const
{
    /*Find all points*/
    std::set<Point*> allPoints;
    _findAllPoints(allPoints);
    /*Print Points, Don't care about the Order*/
    std::set<Point*>::iterator iter = allPoints.begin();
    for (;iter!=allPoints.end(); iter++)
    {
        Point* cur = *iter;
        os << "<"<<GP_XmlString::node<<"_"<<cur<<">/n";
        os << "<"<<GP_XmlString::func<<">";
        if (NULL!=cur->mFunc)
        {
            os << cur->mFunc->name;
        }
        os << "</"<<GP_XmlString::func<<">";
        os << "<"<<GP_XmlString::inputs<<">/n";
        for (INPUT_ITER i=cur->mInputs.begin(); i!=cur->mInputs.end();i++)
        {
            os << GP_XmlString::node<<"_"<<i->first<<"/n";
        }
        os << "</"<<GP_XmlString::inputs<<">/n";
        os << "<"<<GP_XmlString::outputs<<">/n";
        for (OUTPUT_ITER i=cur->mOutputs.begin(); i!=cur->mOutputs.end();i++)
        {
            os << GP_XmlString::node<<"_"<<i->get()<<"/n";
        }
        os << "</"<<GP_XmlString::outputs<<">/n";
        os << "<"<<GP_XmlString::status<<">/n";
        for (STATUS_ITER i=cur->mStatus.begin(); i!=cur->mStatus.end(); i++)
        {
            const IStatusType& s = (*i)->type();
            os << "<"<<s.name()<<">/n";
            s.print(os, (*i)->content());
            os << "</"<<s.name()<<">/n";
        }
        os << "</"<<GP_XmlString::status<<">/n";
        os << "</"<<GP_XmlString::node<<"_"<<cur<<">/n";
    }
}
std::vector<const IStatusType*> GPGraphicADF::vGetInputs() const
{
    return mCacheInputs;
}
std::vector<const IStatusType*> GPGraphicADF::vGetOutputs() const
{
    return mCacheOutputs;
}

GPGraphicADF::GPGraphicADF()
{
}

void GPGraphicADF::_loadMain(const xmlReader::package* root, std::map<std::string, Point*>& allPoints, const GPFunctionDataBase* base) const
{
    const std::vector<xmlReader::package*>& children = root->children;
    std::vector<xmlReader::package*>::const_iterator iter;
    for (iter=children.begin(); iter!=children.end(); iter++)
    {
        const xmlReader::package* node = *iter;
        valid(node->name);
        assert(allPoints.find(node->name)==allPoints.end());//FIXME Print Error instead of assert
        /*Find function name*/
        std::vector<xmlReader::package*>::const_iterator iter_func;
        Point* p = NULL;
        for (iter_func=node->children.begin(); iter_func!=node->children.end(); iter_func++)
        {
            const xmlReader::package* func = *iter_func;
            if (func->name == GP_XmlString::func)
            {
                if (func->attr.empty())//Input or output node
                {
                    p = new Point(NULL);
                }
                else
                {
                    assert(func->attr.size() == 1);//FIXME for debug
                    const GPFunctionDataBase::function* _f = base->vQueryFunction(func->attr[0]);
                    assert(_f!=NULL);
                    p = new Point(_f);
                }
                break;
            }
        }
        assert(NULL!=p);//FIXME Print error instead of assert
        allPoints.insert(std::make_pair(node->name, p));
    }
}

void GPGraphicADF::_loadStatus(const xmlReader::package* attach, const GPFunctionDataBase* base, Point* currentPoint) const
{
    int n = (currentPoint->mFunc->statusType).size();
    assert(n == attach->children.size());
    int cur = 0;
    std::vector<std::istream*> statusContents;
    statusContents.reserve(n);
    for (PITER iter_status=attach->children.begin(); iter_status!=attach->children.end(); iter_status++)
    {
        const xmlReader::package* s = *iter_status;
        std::ostringstream out;
        for (int i=0; i<s->attr.size(); ++i)
        {
            out<<s->attr[i]<<" ";//TODO Modified xmlReader, reduce this combine
        }
        statusContents.push_back(new std::istringstream(out.str()));
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
void GPGraphicADF::_loadInputs(const xmlReader::package* attach, const std::map<std::string, Point*>& allPoints, Point* currentPoint)
{
    for (int i=0; i<attach->attr.size(); ++i)
    {
        Point* p = (allPoints.find(attach->attr[i]))->second;
        currentPoint->connectInput(p);
    }
    if (NULL == currentPoint->mFunc && !attach->attr.empty())
    {
        GPPtr<Point> warp = currentPoint;
        mInputs.push_back(warp);
        currentPoint->addRef();
    }
}
void GPGraphicADF::_loadOutputs(const xmlReader::package* attach, const std::map<std::string, Point*>& allPoints, Point* currentPoint)
{
    for (int i=0; i<attach->attr.size(); ++i)
    {
        Point* p = (allPoints.find(attach->attr[i]))->second;
        p->addRef();
        /*The GPPtr will auto dec reference for p, so add reference before construct GPPtr*/
        GPPtr<Point> warp = p;
        currentPoint->connectOutput(warp);
    }
    if (NULL == currentPoint->mFunc && !attach->attr.empty())
    {
        GPPtr<Point> warp = currentPoint;
        mOutputs.push_back(warp);
        currentPoint->addRef();
    }
}

GPGraphicADF::GPGraphicADF(std::istream& is, const GPFunctionDataBase* base)
{
    xmlReader r;
    const xmlReader::package* root = r.loadStream(is);
    /*Construct all Node firstly*/
    std::map<std::string, Point*> allPoints;
    _loadMain(root, allPoints, base);
    /*Connect all points and Construct status*/
    for (PITER iter=root->children.begin(); iter!=root->children.end(); iter++)
    {
        const xmlReader::package* node = *iter;
        Point* currentPoint = (allPoints.find(node->name))->second;
        for (PITER iter_node=node->children.begin(); iter_node!=node->children.end(); iter_node++)
        {
            const xmlReader::package* attach = *iter_node;
            if (attach->name == GP_XmlString::status)
            {
                _loadStatus(attach, base, currentPoint);
            }
            else if(attach->name == GP_XmlString::inputs)
            {
                _loadInputs(attach, allPoints, currentPoint);
            }
            else if(attach->name == GP_XmlString::outputs)
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
}
GPGraphicADF::~GPGraphicADF()
{
}
