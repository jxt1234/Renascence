#include "user/GPAPI.h"
#include "core/IGPAutoDefFunction.h"
#include "core/GPProducer.h"
#include "core/GPFactory.h"
#include "core/GPFunctionDataBase.h"
#include "evolution/GPADFOptimizorFactory.h"
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

IGPAutoDefFunction* GP_Function_Create_ByType(const AGPProducer* p, const char* outputTypes, const char* inputTypes, bool inputRepeat)
{
    if (NULL == outputTypes || NULL == inputTypes || NULL == p)
    {
        FUNC_PRINT(1);
        return NULL;
    }
    std::vector<const IStatusType*> inputs;
    std::vector<const IStatusType*> outputs;
    {
        std::istringstream is(inputTypes);
        std::string name;
        while(is >> name)
        {
            inputs.push_back(p->F->vQueryType(name));
        }
        is.str(outputTypes);
        while(is >> name)
        {
            outputs.push_back(p->F->vQueryType(name));
        }
    }
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
GP_Output GP_Function_Run(IGPAutoDefFunction* f, const GP_Input& input)
{
    GPASSERT(NULL!=f);//FIXME
    return f->run(input);
}
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
}

IGPAutoDefFunction* GP_Function_CreateBest_ByType(const AGPProducer* p, const char* outputTypes, const char* inputTypes, bool inputRepeat, std::function< double(IGPAutoDefFunction*)> fit_func)
{
    if (NULL == p || NULL ==outputTypes || NULL == inputTypes)
    {
        FUNC_PRINT(1);
        return NULL;
    }
    /*TODO*/
    return NULL;
}
