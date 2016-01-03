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
class AGPContents
{
public:
    AGPContents(bool own = false)
    {
        mOwn = own;
        mContent = new GPContents;
        GPASSERT(NULL!=mContent);
    }
    AGPContents(GPContents* content)
    {
        mOwn = true;
        mContent = content;
    }
    ~AGPContents()
    {
        if (mOwn)
        {
            mContent->clearContents();
        }
        delete mContent;
    }
    bool own() const {return mOwn;}
    GPContents* get() const
    {
        return mContent;
    }
    
private:
    GPContents* mContent;
    bool mOwn;
};


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

AGPContents* GP_Function_Run(IGPAutoDefFunction* f, AGPContents* input)
{
    GPASSERT(NULL!=f);
    if(NULL == input)
    {
        GPContents empty;
        return new AGPContents(f->vRun(&empty));
    }
    return new AGPContents(f->vRun(input->get()));
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
    GPPtr<GPParameter> origin_paramera = origin->getParameters();
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
    auto optfun = [pInfo, origin](GPPtr<GPParameter> para){
        origin->vMap(para.get());
        return pInfo->pFitComputeFunction(origin, pInfo->pMeta);
    };
    int n = origin->vMap(NULL);//Get the count
    GPPtr<GPParameter> result = opt->vFindBest(n, optfun);
    origin->vMap(result.get());
    origin->setParameters(result);
    auto newfit = pInfo->pFitComputeFunction(origin, pInfo->pMeta);
    if (newfit < originfit && origin_paramera.get() != NULL)
    {
        /*Revert to first one*/
        origin->vMap(origin_paramera.get());
        origin->setParameters(origin_paramera);
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

AGPContents* GP_Contents_Load(AGPProducer* p, GPStream** inputs, const char* typeNames, int n)
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
    
    AGPContents* c = new AGPContents(true);
    for (int i=0; i<n; ++i)
    {
        c->get()->push(types[i]->vLoad(inputs[i]), types[i]);
    }
    return c;
}
int GP_Contents_Size(AGPContents* contents)
{
    if (NULL == contents || NULL == contents->get())
    {
        FUNC_PRINT(0);
        return 0;
    }
    return (int)(contents->get()->size());
}


void GP_Contents_Save(AGPContents* content, GPWStream* outputs, int n)
{
    if (NULL == content || NULL == outputs || n <0 || content->get()->size() <= n)
    {
        FUNC_PRINT(0);
        return;
    }
    auto c = content->get()->contents[n];
    c.type->vSave(c.content, outputs);
}


void GP_Contents_Destroy(AGPContents* content)
{
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
    if (NULL == name || "" == std::string(name))
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

static double _FitValue(IGPAutoDefFunction* adf, void* pMeta)
{
    AGPContents* input = (AGPContents*)pMeta;
    GPContents* output = adf->vRun(input->get());
    GPASSERT(output->size() == 1);
    GPASSERT(output->getContent(0).type->name() == "double");
    double res = *((double*)output->get(0));
    GPContents::destroy(output);
    return res;
}
static double _FitTime(IGPAutoDefFunction* adf, void* pMeta)
{
    AGPContents* input = (AGPContents*)pMeta;
    /*TODO, find more accurate method*/
    clock_t sta = clock();
    GPContents* output = adf->vRun(input->get());
    clock_t fin = clock();
    GPContents::destroy(output);
    return 1.0/(fin - sta);
}

GPOptimizorInfo* GP_OptimzorInfo_CreateTemplate(int depth, int maxtimes, int type, AGPContents* pInput)
{
    if (depth<=0 || maxtimes < 1 || (type != GP_OPTIMIZOR_TIME && type !=GP_OPTIMIZOR_VALUE))
    {
        FUNC_PRINT(1);
        return NULL;
    }
    GPOptimizorInfo* info = new GPOptimizorInfo;
    info->nMaxADFDepth = depth;
    info->pMeta = (void*)pInput;
    info->nMaxRunTimes = maxtimes;
    info->nOptimizeType = 0;
    switch (type)
    {
        case GP_OPTIMIZOR_VALUE:
            info->pFitComputeFunction = _FitValue;
            break;
        case GP_OPTIMIZOR_TIME:
            info->pFitComputeFunction = _FitTime;
            break;
    }
    return info;
}

void GP_OptimzorInfo_FreeTemplate(GPOptimizorInfo* pInfo)
{
    GPASSERT(NULL!=pInfo);//FIXME
    delete pInfo;
}


void* GP_Contents_Get(AGPContents* contents, int n)
{
    if (contents==NULL || n<0 || n>=contents->get()->size())
    {
        FUNC_PRINT(0);
        return NULL;
    }
    return contents->get()->get(n);
}

AGPContents* GP_Contents_CreateCollector()
{
    return new AGPContents(false);
}

void GP_Contents_Collect(AGPContents* Collector, AGPContents* B, int n)
{
    GPASSERT(NULL!=B && NULL!=Collector && n>=0 && n<B->get()->size());//FIXME
    Collector->get()->push(B->get()->getContent(n));
}

double GP_Contents_GetDouble(AGPContents* contents, int n)
{
    GPASSERT(NULL!=contents && n>=0 && contents->get()->size()>n);//FIXME
    return *(double*)(contents->get()->get(n));
}

void GP_Contents_SetDouble(AGPContents* contents, double value, int n)
{
    GPASSERT(NULL!=contents && n>=0 && contents->get()->size()>n);//FIXME
    *(double*)(contents->get()->get(n)) = value;
}

AGPStrings* GP_Contents_Types(AGPContents* contents)
{
    GPASSERT(NULL!=contents);//FIXME
    std::ostringstream types;
    for (int i=0; i<contents->get()->size(); ++i)
    {
        types << contents->get()->getContent(i).type->name() << " ";
    }
    AGPStrings* s = new AGPStrings;
    s->a.push_back(types.str());
    return s;
}

AGPContents* GP_Contents_CreateDouble(double value)
{
    GPContents* content = new GPContents;
    double* v = new double;
    *v = value;
    content->push(v, gDefaultDoubleType);
    return new AGPContents(content);
}

