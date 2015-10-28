#include "core/GPFunctionTree.h"
GPFunctionTree::GPFunctionTree(const GPFunctionDataBase::function* f)
{
    GPASSERT(NULL!=f);
    mF = f;
    mInputNum = -1;
}
GPFunctionTree::GPFunctionTree(size_t n)
{
    GPASSERT(n>=0);
    mInputNum = n;
    mF = NULL;
}
GPFunctionTree::~GPFunctionTree()
{
}
