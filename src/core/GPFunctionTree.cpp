#include "core/GPFunctionTree.h"
GPFunctionTreePoint::GPFunctionTreePoint(const GPFunctionDataBase::function* f, int inputNumber)
{
    GPASSERT(NULL != f || inputNumber >=0);
    mF = f;
    mInputNumber = inputNumber;
}
GPFunctionTreePoint::~GPFunctionTreePoint()
{
}


GPFunctionTree::GPFunctionTree(GPPtr<GPFunctionTreePoint> root, bool totalVariable)
{
    mRoot = root;
    if (totalVariable)
    {
        mVariableSubTree.push_back(root.get());
    }
}
GPFunctionTree::~GPFunctionTree()
{
}

void GPFunctionTree::addVariableSubTree(GPFunctionTreePoint* subtree)
{
    /*TODO find other way to validate*/
    auto points = mRoot->display();
    bool issubtree = false;
    for (auto p : points)
    {
        if (p == subtree)
        {
            issubtree = true;
            break;
        }
    }
    GPASSERT(issubtree);
    for (auto p : mVariableSubTree)
    {
        for (auto pp : p->display())
        {
            if (pp == subtree)
            {
                GPASSERT(0);//FIXME
                return;
            }
        }
    }
    mVariableSubTree.push_back(subtree);
}

class GPFunctionTreePointCopy:public GPAbstractPoint::IPointCopy
{
public:
    virtual GPAbstractPoint* copy(GPAbstractPoint* src)
    {
        GPFunctionTreePoint* _src = (GPFunctionTreePoint*)src;
        GPFunctionTreePoint* res = new GPFunctionTreePoint(_src->function(), _src->inputNumber());
        return res;
    }
private:
};


GPFunctionTree* GPFunctionTree::copy(const GPFunctionTree* origin)
{
    GPFunctionTreePointCopy copyt;
    GPFunctionTreePoint* p = (GPFunctionTreePoint*)(GPAbstractPoint::deepCopy(origin->root(), &copyt));
    return new GPFunctionTree(p);
}
