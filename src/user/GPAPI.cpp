#include <string>
#include <sstream>
#include "platform/system_lib.h"
#include "core/IGPAutoDefFunction.h"
#include "core/GPProducer.h"
#include "core/GPFactory.h"
#include "evolution/GPEvolutionGroup.h"
#include "optimizor/GPOptimizorFactory.h"
#include "AGPProducer.h"
#include "user/GPAPI.h"
#include "xml/xmlReader.h"
#include "core/GPStreamFactory.h"
void GP_Set_Lib_Path(const char* basic_path)
{
    if (NULL == basic_path)
    {
        return;
    }
    system_set_path(basic_path);
}

AGPProducer* GP_Producer_Create(GPStream** metaStream, IFunctionTable** table, int n, int type)
{
    FUNC_PRINT(type);
    if (n <= 0 || NULL == metaStream || NULL == table)
    {
        GPPRINT("InValid parameters!!");
        return NULL;
    }
    GPFunctionDataBase* f = new GPFunctionDataBase;
    if (NULL == f)
    {
        FUNC_PRINT_ALL(f, p);
        return NULL;
    }
    AUTOUNREF(f);
    for (int i=0; i<n; ++i)
    {
        if (NULL == metaStream[i])
        {
            GPPRINT("InValid stream in GP_Producer_Create");
            return NULL;
        }
        f->loadXml(metaStream[i], table[i]);
    }
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

GPContents* GP_Function_Run(IGPAutoDefFunction* f, GPContents* input)
{
    GPASSERT(NULL!=f);
    if(NULL == input)
    {
        GPContents empty;
        return f->vRun(&empty);
    }
    return f->vRun(input);
}

IGPAutoDefFunction* GP_Function_Create_ByStream(const AGPProducer* p, GPStream* xmlFile)
{
    if (NULL == p)
    {
        FUNC_PRINT(1);
        return NULL;
    }
    xmlReader r;
    const GPTreeNode* root = r.loadStream(xmlFile);
    return p->P->vCreateFunctionFromNode(root);
}
void GP_Function_Save(IGPAutoDefFunction* f, GPWStream* output)
{
    GPASSERT(NULL!=f);//FIXME
    GPPtr<GPTreeNode> node = f->vSave();
    xmlReader::dumpNodes(node.get(), output);
}

void GP_Function_Destroy(IGPAutoDefFunction* f)
{
    if (NULL!=f)
    {
        f->decRef();
    }
}

void GP_Function_Optimize(IGPAutoDefFunction* f, std::function< double(IGPAutoDefFunction*)> fit_fun, int type, const char* describe)
{
    /*TODO*/
    GPPtr<IGPOptimizor> opt;
    switch(type)
    {
        case 0:
            opt = GPOptimizorFactory::create(GPOptimizorFactory::PSO_SEARCH, describe);
            break;
        case 1:
            opt = GPOptimizorFactory::create(GPOptimizorFactory::GOLDEN_DIVIDE, describe);
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
    const int size = 20;
    int gens = maxTimes/size;
    if (gens < 1)
    {
        gens = 1;
    }
    GPPtr<GPEvolutionGroup> group = new GPEvolutionGroup(p->P, gens, size);//FIXME Set it from parameter
    std::vector<const IStatusType*> inputs = _transform(inputTypes, p);
    std::vector<const IStatusType*> outputs = _transform(outputTypes, p);
    group->vSetInput(inputs);
    group->vSetOutput(outputs);
    group->vEvolutionFunc(fit_func);
    auto best = group->getBest();
    best->addRef();
    return best;
}


GPStream* GP_Stream_Create(const char* file)
{
    return GPStreamFactory::NewStream(file);
}
void GP_Stream_Destroy(GPStream* s)
{
    s->release();
    delete s;
}

GPWStream* GP_WStream_Create(const char* file)
{
    return GPStreamFactory::NewWStream(file);
}
void GP_WStream_Destroy(GPWStream* s)
{
    s->release();
    delete s;
}

void GP_Set_Stream_Path(const char* basic_path)
{
    GPStreamFactory::setParentPath(basic_path);
}

GPContents* GP_Contents_Load(AGPProducer* p, GPStream** inputs, const char* typeNames, int n)
{
    if (NULL == p || NULL == inputs || NULL == typeNames || 0 == n)
    {
        FUNC_PRINT(0);
        return NULL;
    }
    auto types = _transform(typeNames, p);
    if (types.size()!=n)
    {
        FUNC_PRINT(0);
        return NULL;
    }
    for (int i=0; i<n; ++i)
    {
        if (NULL == inputs[i])
        {
            FUNC_PRINT(0);
            return NULL;
        }
    }
    
    GPContents* c = new GPContents;
    for (int i=0; i<n; ++i)
    {
        c->push(types[i]->vLoad(inputs[i]), types[i]);
    }
    return c;
}

void GP_Contents_Save(GPContents* content, GPWStream** outputs, int n)
{
    if (NULL == content || NULL == outputs || n <= 0 || content->size() != n)
    {
        FUNC_PRINT(0);
        return;
    }
    for (int i=0; i<n; ++i)
    {
        if (NULL == outputs[i])
        {
            FUNC_PRINT(0);
            return;
        }
    }
    for (int i=0; i<n; ++i)
    {
        auto c = content->contents[i];
        c.type->vSave(c.content, outputs[i]);
    }
}


void GP_Contents_Destroy(GPContents* content)
{
    content->clear();
    delete content;
}

