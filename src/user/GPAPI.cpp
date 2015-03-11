#include "user/GPAPI.h"
#include "core/IGPAutoDefFunction.h"
#include "core/GPProducer.h"
#include "core/GPFactory.h"
#include "core/GPFunctionDataBase.h"
#include "evolution/GPEvolutionGroup.h"
#include "optimizor/GPOptimizorFactory.h"
#include <string>
#include <sstream>
/*Warp for GPFunctionDataBase and GPProducer*/
class AGPProducer:public RefCount
{
    public:
        AGPProducer(GPFunctionDataBase* f, GPProducer* p)
        {
            GPASSERT(NULL!=f);
            GPASSERT(NULL!=p);
            f->addRef();
            p->addRef();
            P=p;
            F=f;
        }
        virtual ~AGPProducer()
        {
            P->decRef();
            F->decRef();
        }
        GPProducer* P;
        GPFunctionDataBase* F;
};

AGPProducer* GP_Producer_Create(std::istream& metaStream, IFunctionTable* table, int type)
{
    FUNC_PRINT(type);
    GPFunctionDataBase* f = new GPFunctionDataBase;
    if (NULL == f)
    {
        FUNC_PRINT_ALL(f, p);
        return NULL;
    }
    AUTOUNREF(f);
    f->loadXml(metaStream, table);
    GPProducer* p = NULL;
    switch(type)
    {
        case GP_PRODUCER_TREE:
            p = GPFactory::createProducer(f, GPFactory::TREE);
            break;
        case GP_PRODUCER_GRAPHIC:
            p = GPFactory::createProducer(f, GPFactory::GRAPHIC);
            break;
        default:
            break;
    }
    if (NULL == p)
    {
        FUNC_PRINT_ALL(p, p);
        return NULL;
    }
    AUTOUNREF(p);
    AGPProducer* res = new AGPProducer(f, p);
    return res;
}
void GP_Producer_Destroy(AGPProducer* p)
{
    FUNC_PRINT_ALL(p, p);
    if (NULL == p)
    {
        FUNC_PRINT(1);
        return;
    }
    p->decRef();
}

static std::vector<const IStatusType*> _transform(const char* types, const AGPProducer* p)
{
    GPASSERT(NULL!=types);
    GPASSERT(NULL!=p);
    std::istringstream is(types);
    std::string name;
    std::vector<const IStatusType*> res;
    while(is >> name)
    {
        auto t = p->F->vQueryType(name);
        if (NULL!=t)
        {
            res.push_back(t);
        }
    }
    return res;
}

IGPAutoDefFunction* GP_Function_Create_ByType(const AGPProducer* p, const char* outputTypes, const char* inputTypes, bool inputRepeat)
{
    if (NULL == outputTypes || NULL == inputTypes || NULL == p)
    {
        FUNC_PRINT(1);
        return NULL;
    }
    std::vector<const IStatusType*> inputs = _transform(inputTypes, p);
    std::vector<const IStatusType*> outputs = _transform(outputTypes, p);
    return p->P->vCreateFunction(outputs, inputs, inputRepeat);
}
IGPAutoDefFunction* GP_Function_Create_ByFormula(const AGPProducer* p, const char* formula)
{
    if (NULL == formula || NULL == p)
    {
        FUNC_PRINT(1);
        return NULL;;
    }
    return p->P->vCreateFunctionFromFormula(std::string(formula));
}

std::vector<const IStatusType*> GP_Function_Get_Inputs(const IGPAutoDefFunction* f)
{
    GPASSERT(NULL!=f);//FIXME
    return f->vGetInputs();
}


std::vector<const IStatusType*> GP_Function_Get_Outputs(const IGPAutoDefFunction* f)
{
    GPASSERT(NULL!=f);//FIXME
    return f->vGetOutputs();
}
/*
GP_Output GP_Function_Run(IGPAutoDefFunction* f, const GP_Input& input)
{
    GPASSERT(NULL!=f);//FIXME
    return f->run(input);
}
 */
IGPAutoDefFunction* GP_Function_Create_ByStream(const AGPProducer* p, std::istream& xmlFile)
{
    if (NULL == p)
    {
        FUNC_PRINT(1);
        return NULL;
    }
    return p->P->vCreateFunctionFromIS(xmlFile);
}
void GP_Function_Save(IGPAutoDefFunction* f, std::ostream& output)
{
    GPASSERT(NULL!=f);//FIXME
    f->save(output);
}

void GP_Function_Destroy(IGPAutoDefFunction* f)
{
    if (NULL!=f)
    {
        f->decRef();
    }
}

void GP_Function_Optimize(IGPAutoDefFunction* f, std::function< double(IGPAutoDefFunction*)> fit_fun, int type, int maxTimes)
{
    /*TODO*/
    GPPtr<IGPOptimizor> opt;
    switch(type)
    {
        case 0:
            opt = GPOptimizorFactory::create(GPOptimizorFactory::PSO_SEARCH);
            break;
        case 1:
            opt = GPOptimizorFactory::create(GPOptimizorFactory::GOLDEN_DIVIDE);
            break;
        default:
            break;
    }
    if (NULL == opt.get())
    {
        FUNC_PRINT(1);
        return;
    }
    auto optfun = [&](GPPtr<GPParameter> para){
        f->vMap(para);
        return fit_fun(f);
    };
    GPPtr<GPParameter> result;
    int n = f->vMap(result);//Get the count
    result = opt->vFindBest(n, optfun);
    f->vMap(result);
}

IGPAutoDefFunction* GP_Function_CreateBest_ByType(const AGPProducer* p, const char* outputTypes, const char* inputTypes, bool inputRepeat, std::function< double(IGPAutoDefFunction*)> fit_func, int maxTimes)
{
    if (NULL == p || NULL ==outputTypes || NULL == inputTypes)
    {
        FUNC_PRINT(1);
        return NULL;
    }
    GPPtr<GPEvolutionGroup> group = new GPEvolutionGroup(p->P, 100, 20);//FIXME Set it from parameter
    std::vector<const IStatusType*> inputs = _transform(inputTypes, p);
    std::vector<const IStatusType*> outputs = _transform(outputTypes, p);
    group->vSetInput(inputs);
    group->vSetOutput(outputs);
    group->vEvolutionFunc(fit_func);
    auto best = group->getBest();
    best->addRef();
    return best;
}
