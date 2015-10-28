#include "core/GPFunctionTree.h"
GPFunctionTree::GPFunctionTree(const GPFunctionDataBase::function* f)
{
    GPASSERT(NULL!=f);
    mF = f;
    mT = NULL;
}
GPFunctionTree::GPFunctionTree(const IStatusType* type)
{
    GPASSERT(NULL!=type);
    mT = type;
    mF = NULL;
}
GPFunctionTree::~GPFunctionTree()
{
}
