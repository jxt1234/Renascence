#include "xml/xmlTree.h"
#include "core/GP_XmlString.h"
#include <sstream>
using namespace std;

void xmlTree::printBefore(ostream& out)
{
    out << "<"<<mFunc<<">"<<endl;
    out << "<"<<GP_XmlString::result<<">"<<endl;
    printValues(mResult, out);
    out << "</"<<GP_XmlString::result<<">"<<endl;
    out << "<"<<GP_XmlString::status<<">"<<endl;
    printValues(mStatus, out);
    out << "</"<<GP_XmlString::status<<">"<<endl;
}

void xmlTree::printAfter(std::ostream& out)
{
    out << "</"<<mFunc<<">"<<endl;
}

void xmlTree::printValues(std::vector<type>& result, std::ostream& out)
{
    for (int i=0; i<result.size(); ++i)
    {
        out << "<"<<result[i].name<<">"<<endl;
        out << result[i].content<<endl;
        out << "</"<<result[i].name<<">"<<endl;
    }
}

void xmlTree::attributeUnflatten()
{
    loadNode(mAttributes);
}

void xmlTree::loadNode(xmlReader::package* p)
{
    /*Find Function*/
    for (int i=0; i<p->children.size(); ++i)
    {
        xmlReader::package* c = p->children[i];
        if (c->name == GP_XmlString::func)
        {
            mFunc = c->attr[0];
            break;
        }
    }
    /*Load Result and status*/
    for (int i=0; i<p->children.size(); ++i)
    {
        xmlReader::package* c = p->children[i];
        if (c->name == GP_XmlString::result)
        {
            loadValues(mResult, c);
        }
        else if(c->name == GP_XmlString::status)
        {
            loadValues(mStatus, c);
        }
    }
    /*Load Childern*/
    xmlReader::package* _children = NULL;
    for (int i=0; i<p->children.size(); ++i)
    {
        xmlReader::package* c = p->children[i];
        if (c->name == GP_XmlString::children)
        {
            _children = c;
            break;
        }
    }
    if (NULL!=_children)
    {
        for (int i=0; i<_children->children.size(); ++i)
        {
            xmlReader::package* c = _children->children[i];
            xmlTree* child = new xmlTree;
            addPoint(child);
            child->loadNode(c);
        }
    }
}

void xmlTree::loadValues(std::vector<type>& result, xmlReader::package* p)
{
    for (int i=0; i<p->children.size(); ++i)
    {
        xmlReader::package* c = p->children[i];
        type t;
        t.name = c->name;
        ostringstream out;
        for (int i=0; i<c->attr.size(); ++i)
        {
            out << c->attr[i]<<" ";
            t.content = out.str();
        }
        result.push_back(t);
    }
}
