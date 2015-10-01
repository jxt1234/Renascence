#include "xml/xmlTree.h"
#include "utils/debug.h"
#include "GPStrings.h"
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
    GPASSERT(p->name() == GPStrings::GPTreeADF_FUNCTION);
    GPASSERT(""!=p->attr());
    mFunc = p->attr();
    auto childrens = p->getChildren();
    GPASSERT(2 == childrens.size() || 4 == childrens.size());
    auto statusnode = childrens[0];
    GPASSERT(statusnode->name()== GPStrings::GPTreeADF_STATUS);
    /*Load status*/
    loadValues(mStatus, statusnode.get());
    /*Load Childern*/
    auto childnodes = childrens[1];
    GPASSERT(childnodes->name()==GPStrings::GPTreeADF_CHILDREN);
    for (auto c : childnodes->getChildren())
    {
        addPoint(new xmlTree(c.get()));
    }
    if (4 == childrens.size())
    {
        mInputTypes = childrens[2]->attr();
        mChildFlags = childrens[3]->attr();
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
