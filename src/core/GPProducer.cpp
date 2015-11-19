#include "core/GPProducer.h"
#include "backend/GPBackEndProducer.h"
#include "frontend/GPFrontEndProducer.h"

GPProducer::GPProducer(GPFrontEndProducer* front, GPBackEndProducer* back, const GPFunctionDataBase* base)
{
    GPASSERT(NULL!=front);
    GPASSERT(NULL!=back);
    GPASSERT(NULL!=base);
    mFront = front;
    mBack = back;
    mFront->addRef();
    mBack->addRef();
    mBase = base;
}
GPProducer::~GPProducer()
{
    mFront->decRef();
    mBack->decRef();
}

IGPAutoDefFunction* GPProducer::createFunction(const std::vector<const IStatusType*>& outputs, const std::vector<const IStatusType*>& inputs)
{
    GPPtr<GPFunctionTree> ft = mFront->vCreateOneFunction(outputs,inputs);
    return mBack->vCreateFromFuncTree(ft.get());
}
std::vector<GPPtr<IGPAutoDefFunction>> GPProducer::listAllFunction(const std::vector<const IStatusType*>& outputs, const std::vector<const IStatusType*>& inputs)
{
    auto fts = mFront->vCreateAllFunction(outputs, inputs);
    std::vector<GPPtr<IGPAutoDefFunction> > result;
    for (auto f : fts)
    {
        result.push_back(mBack->vCreateFromFuncTree(f));
        f->decRef();
    }
    return result;
}
IGPAutoDefFunction* GPProducer::createFunction(const std::string& formula)
{
    GPPtr<GPFunctionTree> ft = mFront->vCreateFromFormula(formula);
    return mBack->vCreateFromFuncTree(ft.get());
}
IGPAutoDefFunction* GPProducer::createFunction(const GPTreeNode* node)
{
    return mBack->vCreateFromNode(node, mBase);
}

std::vector<GPPtr<GPProducer::FunctionWrap>> GPProducer::listAllFunctionWithBackUp(const std::vector<const IStatusType*>& outputs, const std::vector<const IStatusType*>& inputs)
{
    std::vector<GPPtr<GPProducer::FunctionWrap>> result;
    auto fts = mFront->vCreateAllFunction(outputs, inputs);
    for (auto f : fts)
    {
        result.push_back(new FunctionWrap(mBack->vCreateFromFuncTree(f), f));
    }
    return result;
}

