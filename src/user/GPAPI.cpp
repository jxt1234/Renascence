#include <string>
#include <sstream>
#include "platform/system_lib.h"
#include "core/IGPAutoDefFunction.h"
#include "core/GPProducer.h"
#include "core/GPFactory.h"
#include "frontend/GPFrontEndProducer.h"
#include "backend/GPBackEndProducer.h"
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
    if (n <= 0 || NULL == metaStream)
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
        if (NULL == table)
        {
            f->loadXml(metaStream[i]);
        }
        else
        {
            f->loadXml(metaStream[i], table[i]);
        }
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
        case GP_PRODUCER_STREAM:
            p = GPFactory::createProducer(f, GPFactory::STREAM);
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

IGPAutoDefFunction* GP_Function_Create_ByType(const AGPProducer* p, const char* outputTypes, const char* inputTypes, GPOptimizorInfo* pInfo)
{
    if (NULL == outputTypes || NULL == inputTypes || NULL == p)
    {
        FUNC_PRINT(1);
        return NULL;
    }
    std::vector<const IStatusType*> inputs = _transform(inputTypes, p);
    std::vector<const IStatusType*> outputs = _transform(outputTypes, p);
    if (NULL == pInfo)
    {
        return p->P->createFunction(outputs, inputs);
    }
    GPPtr<GPEvolutionGroup> group = new GPEvolutionGroup(p->P, pInfo->nMaxRunTimes/20, 20);
    group->vSetInput(inputs);
    group->vSetOutput(outputs);
    auto fit_func = [pInfo](IGPAutoDefFunction* f){
        return pInfo->pFitComputeFunction(f, pInfo->pMeta);
    };
    group->vEvolutionFunc(fit_func);
    auto best = group->getBest();
    best->addRef();
    return best.get();

}
IGPAutoDefFunction* GP_Function_Create_ByFormula(const AGPProducer* p, const char* formula, const char* inputType, GPOptimizorInfo* pInfo)
{
    if (NULL == formula || NULL == p)
    {
        FUNC_PRINT(1);
        return NULL;;
    }
    if (NULL == pInfo)
    {
        return p->P->createFunction(std::string(formula), _transform(inputType, p));
    }
    GPPtr<GPFunctionTree> first = p->P->getFront()->vCreateFromFormula(formula, _transform(inputType, p));
    if (first->getVariable().empty())
    {
        auto result = p->P->getBack()->vCreateFromFuncTree(first.get());
        result->setBasicTree(first);
        int n = result->vMap(NULL);
        if (n > 0)
        {
            GPPtr<GPParameter> p = new GPParameter(result->vMap(NULL));
            result->vMap(p.get());
            result->setParameters(p);
        }
        return result;
    }
    GPPtr<GPEvolutionGroup> group = new GPEvolutionGroup(p->P, pInfo->nMaxRunTimes/20, 20);
    auto fit_func = [pInfo](IGPAutoDefFunction* f){
        return pInfo->pFitComputeFunction(f, pInfo->pMeta);
    };
    group->setBestTree(first);
    group->vEvolutionFunc(fit_func);
    auto best = group->getBest();
    best->addRef();
    return best.get();
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
    return p->P->createFunction(root);
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

void GP_Function_Optimize(IGPAutoDefFunction* origin, GPOptimizorInfo* pInfo)
{
    if (NULL == pInfo || NULL == origin)
    {
        FUNC_PRINT(1);
        return;
    }
    if (NULL == pInfo->pMeta || NULL == pInfo->pFitComputeFunction)
    {
        FUNC_PRINT(1);
        return;
    }
    GPPtr<IGPOptimizor> opt;
    GPPtr<IGPAutoDefFunction> f = origin->vCopy();
    double originfit = pInfo->pFitComputeFunction(origin, pInfo->pMeta);
    switch(pInfo->nOptimizeType)
    {
        case 0:
            opt = GPOptimizorFactory::create(GPOptimizorFactory::PSO_SEARCH, pInfo->nMaxRunTimes);
            break;
        case 1:
            opt = GPOptimizorFactory::create(GPOptimizorFactory::GOLDEN_DIVIDE, pInfo->nMaxRunTimes);
            break;
        default:
            break;
    }
    if (NULL == opt.get())
    {
        FUNC_PRINT(1);
        return;
    }
    auto optfun = [pInfo, f](GPPtr<GPParameter> para){
        f->vMap(para.get());
        return pInfo->pFitComputeFunction(f.get(), pInfo->pMeta);
    };
    GPPtr<GPParameter> result;
    int n = f->vMap(result.get());//Get the count
    result = opt->vFindBest(n, optfun);
    f->vMap(result.get());
    auto newfit = pInfo->pFitComputeFunction(f.get(), pInfo->pMeta);
    if (newfit > originfit)
    {
        origin->vMap(result.get());
        origin->setParameters(result);
    }
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


GPStream** GP_Streams_Malloc(int n)
{
    if (n <=0 || n>10000)
    {
        FUNC_PRINT(1);
        return NULL;
    }
    return (GPStream**)malloc(sizeof(GPStream*)*n);
}
void GP_Streams_Free(GPStream** streams)
{
    if (NULL == streams)
    {
        FUNC_PRINT(1);
        return;
    }
    free(streams);
}
GPStream* GP_Streams_Get(GPStream** streams, int n)
{
    if (NULL == streams || n<0)
    {
        FUNC_PRINT(1);
        return NULL;
    }
    return streams[n];
}
void GP_Streams_Set(GPStream** streams,GPStream* contents, int n)
{
    if (NULL == streams || n<0)
    {
        FUNC_PRINT(1);
        return;
    }
    streams[n] = contents;
}

struct AGPStrings
{
    std::vector<std::string> a;
};

int GP_Strings_Number(AGPStrings* strings)
{
    GPASSERT(NULL!=strings);
    return (int)strings->a.size();
}
void GP_Strings_Free(AGPStrings* strings)
{
    GPASSERT(NULL!=strings);
    delete strings;
}

const char* GP_Strings_Get(AGPStrings* strings, int n)
{
    GPASSERT(strings!=NULL);
    GPASSERT(0<=n && n<strings->a.size());
    return strings->a[n].c_str();
}
AGPStrings* GP_Function_GetFormula(IGPAutoDefFunction* f, const char* name)
{
    const GPFunctionTree* tree = f->getBasicTree().get();
    GPASSERT(NULL!=tree);
    if (NULL == name)
    {
        AGPStrings* result = new AGPStrings;
        result->a.push_back(tree->dump());
        return result;
    }
    auto iter = tree->getSubTreeName().find(name);
    if (iter == tree->getSubTreeName().end())
    {
        FUNC_PRINT(1);
        return NULL;
    }
    std::ostringstream tempoutput;
    iter->second->render(tempoutput);
    AGPStrings* result = new AGPStrings;
    result->a.push_back(tempoutput.str());
    return result;
}
AGPStrings* GP_Function_GetParameters(IGPAutoDefFunction* f)
{
    GPASSERT(NULL!=f);//FIXME
    GPPtr<GPParameter> p = f->getParameters();
    GPASSERT(NULL!=p.get());//FIXME
    auto n = p->size();
    std::ostringstream tempoutput;
    for (int i=0; i<n; ++i)
    {
        tempoutput << p->get(i) << " ";
    }
    AGPStrings* result = new AGPStrings;
    result->a.push_back(tempoutput.str());
    return result;
    
}
AGPStrings* GP_Producer_ListFunctions(AGPProducer* producer)
{
    GPASSERT(NULL!=producer);//FIXME
    auto functions = producer->F->getAllFunctions();
    AGPStrings* result = new AGPStrings;
    for (auto f : functions)
    {
        if (f->shortname!="")
        {
            result->a.push_back(f->shortname);
        }
        else
        {
            result->a.push_back(f->name);
        }
    }
    return result;
}
AGPStrings* GP_Producer_ListTypes(AGPProducer* producer)
{
    GPASSERT(NULL!=producer);//FIXME
    auto types = producer->F->getAllType();
    AGPStrings* result = new AGPStrings;
    for (auto t : types)
    {
        result->a.push_back(t->name());
    }
    return result;
}

