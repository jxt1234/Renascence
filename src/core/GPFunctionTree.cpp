#include "core/GPFunctionTree.h"
GPFunctionTree::GPFunctionTree(const GPFunctionDataBase::function* f, size_t inputNumber)
{
    GPASSERT(NULL == f && inputNumber <0);
    mF = f;
    mInputNumber = inputNumber;
}
GPFunctionTree::~GPFunctionTree()
{
}
