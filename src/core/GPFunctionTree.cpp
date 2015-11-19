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


GPFunctionTree::GPFunctionTree(GPPtr<GPFunctionTreePoint> root)
{
    mRoot = root;
}
GPFunctionTree::~GPFunctionTree()
{
    
}
