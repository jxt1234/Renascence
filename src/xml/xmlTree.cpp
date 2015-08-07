#include "xml/xmlTree.h"
#include "utils/debug.h"
#include "core/GP_XmlString.h"
#include <sstream>
using namespace std;


void xmlTree::printValues(std::vector<type>& result, std::ostream& out)
{
    for (int i=0; i<result.size(); ++i)
    {
        out << "<"<<result[i].name<<">"<<endl;
        out << result[i].content<<endl;
        out << "</"<<result[i].name<<">"<<endl;
    }
}

xmlTree::xmlTree(const GPTreeNode* p)
{
    GPASSERT(p->name() == GP_XmlString::func);
    GPASSERT(""!=p->attr());
    mFunc = p->attr();
    auto childrens = p->getChildren();
    GPASSERT(2 == childrens.size());
    auto statusnode = childrens[0];
    GPASSERT(statusnode->name()== GP_XmlString::status);
    /*Load status*/
    loadValues(mStatus, statusnode.get());
    /*Load Childern*/
    auto childnodes = childrens[1];
    GPASSERT(childnodes->name()==GP_XmlString::children);
    for (auto c : childnodes->getChildren())
    {
        addPoint(new xmlTree(c.get()));
    }
}

void xmlTree::loadValues(std::vector<type>& result, const GPTreeNode* p)
{
    for (auto c : p->getChildren())
    {
        type t;
        t.name = c->name();
        t.content = c->attr();
        result.push_back(t);
    }
}
