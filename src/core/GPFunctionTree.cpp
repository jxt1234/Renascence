#include "core/GPFunctionTree.h"
GPFunctionTree::GPFunctionTree(const GPFunctionDataBase::function* f, int inputNumber)
{
    GPASSERT(NULL != f || inputNumber >=0);
    mF = f;
    mInputNumber = inputNumber;
}
GPFunctionTree::~GPFunctionTree()
{
}
