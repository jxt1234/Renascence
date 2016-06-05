/******************************************************************
   Copyright 2016, Jiang Xiao-tang

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
******************************************************************/
#include <string>
#include <sstream>
#include <string.h>
#include "platform/system_lib.h"
#include "core/IGPAutoDefFunction.h"
#include "core/GPProducer.h"
#include "core/GPFactory.h"
#include "core/GPPieceFactory.h"
#include "frontend/GPFrontEndProducer.h"
#include "backend/GPBackEndProducer.h"
#include "evolution/GPEvolutionGroup.h"
#include "optimizor/GPOptimizorFactory.h"
#include "AGPProducer.h"
#include "AGPPiecesProducer.h"
#include "user/GPAPI.h"
#include "xml/xmlReader.h"
#include "core/GPStreamFactory.h"


#include "lowlevelAPI/GPParallelType.h"
#include "lowlevelAPI/IParallelMachine.h"
#include "lowlevelAPI/GPPieces.h"

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
    GPASSERT(NULL!=p);
    GPASSERT(NULL!=types);
    std::vector<const IStatusType*> res;
    std::istringstream is(types);
    std::string name;
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
    GPPtr<GPEvolutionGroup> group = new GPEvolutionGroup(p->P, pInfo->nMaxRunTimes/20, 20, pInfo->nMaxADFDepth);
    group->vSetInput(inputs);
    group->vSetOutput(outputs);
    auto fit_func = [pInfo](IGPAutoDefFunction* f){
        return pInfo->pFitComputeFunction(f, pInfo->pMeta);
    };
    group->vEvolutionFunc(fit_func, pInfo->pBestInfo);
    auto best = group->getBest();
    best->addRef();
    pInfo->fOutputBest = group->getBestFit();
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
    int evolutiontime = pInfo->nMaxRunTimes/20;
    if (evolutiontime < 1)
    {
        evolutiontime = 1;
    }
    GPPtr<GPEvolutionGroup> group = new GPEvolutionGroup(p->P, evolutiontime, 20, pInfo->nMaxADFDepth);
    auto fit_func = [pInfo](IGPAutoDefFunction* f){
        return pInfo->pFitComputeFunction(f, pInfo->pMeta);
    };
    group->setBestTree(first);
    group->vEvolutionFunc(fit_func, pInfo->pBestInfo);
    auto best = group->getBest();
    best->addRef();
    pInfo->fOutputBest = group->getBestFit();
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
    delete s;
}

GPWStream* GP_WStream_Create(const char* file)
{
    return GPStreamFactory::NewWStream(file);
}
void GP_WStream_Destroy(GPWStream* s)
{
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
int GP_Contents_Size(GPContents* contents)
{
    if (NULL == contents)
    {
        FUNC_PRINT(0);
        return 0;
    }
    return (int)(contents->size());
}


void GP_Contents_Save(GPContents* content, GPWStream* outputs, int n)
{
    if (NULL == content || NULL == outputs || n <0 || content->size() <= n)
    {
        FUNC_PRINT(0);
        return;
    }
    auto type = content->getType(n);
    auto content_ = content->get(n);
    type->vSave(content_, outputs);
}


void GP_Contents_Destroy(GPContents* content)
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

struct GPTemplateMeta
{
    GPContents* pInput;
    IGPAutoDefFunction* pPostFunction;
    GPContents* pPostExtraInput;
};


static double _FitValue(IGPAutoDefFunction* adf, void* pMeta)
{
    GPTemplateMeta* meta = (GPTemplateMeta*)pMeta;
    GPContents* input = meta->pInput;
    GPContents* output = adf->vRun(input);
    if (NULL == meta->pPostFunction)
    {
        GPASSERT(output->size() == 1);
        GPASSERT(output->getType(0)->name() == "double");
        double res = *((double*)output->get(0));
        delete output;
        return res;
    }
    GPContents* secondInput = new GPContents;
    secondInput->pushContent(output->getContent(0));
    if (NULL!=meta->pPostExtraInput)
    {
        secondInput->merge(*(meta->pPostExtraInput));
    }
    GPContents* postOutput = meta->pPostFunction->vRun(secondInput);
    delete secondInput;
    delete output;
    GPASSERT(postOutput->size() == 1);
    GPASSERT(postOutput->getType(0)->name() == "double");
    double res = *((double*)postOutput->get(0));
    delete postOutput;
    return res;
}
static double _FitTime(IGPAutoDefFunction* adf, void* pMeta)
{
    GPTemplateMeta* meta = (GPTemplateMeta*)pMeta;
    GPContents* input = meta->pInput;
    GPContents* postOutput = NULL;
    /*TODO, find more accurate method*/
    clock_t sta = clock();
    GPContents* output = adf->vRun(input);
    if (NULL != meta->pPostFunction)
    {
        GPContents* secondInput = new GPContents;
        secondInput->pushContent(output->getContent(0));
        if (NULL!=meta->pPostExtraInput)
        {
            secondInput->merge(*(meta->pPostExtraInput));
        }
        postOutput = meta->pPostFunction->vRun(secondInput);
        delete secondInput;
    }
    clock_t fin = clock();
    delete output;
    if (NULL != postOutput)
    {
        delete postOutput;
    }
    return 1.0/(fin - sta);
}


GPOptimizorInfo* GP_OptimzorInfo_CreateTemplate(int depth, int maxtimes, int type, GPContents* pInput, GPWStream* bestCache, IGPAutoDefFunction* pPostFunction, GPContents* pPostExtraInput)
{
    if (depth<0 || maxtimes < 1 || (type != GP_OPTIMIZOR_TIME && type !=GP_OPTIMIZOR_VALUE))
    {
        FUNC_PRINT(1);
        return NULL;
    }
    GPOptimizorInfo* info = new GPOptimizorInfo;
    GPTemplateMeta* meta = new GPTemplateMeta;
    meta->pPostFunction = pPostFunction;
    meta->pInput = pInput;
    meta->pPostExtraInput = pPostExtraInput;
    info->nMaxADFDepth = depth;
    info->pMeta = (void*)meta;
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
    info->pBestInfo = bestCache;
    info->fTargetBestValue = 0.0f;
    return info;
}

void GP_OptimzorInfo_FreeTemplate(GPOptimizorInfo* pInfo)
{
    GPASSERT(NULL!=pInfo);//FIXME
    GPTemplateMeta* meta = (GPTemplateMeta*)(pInfo->pMeta);
    delete meta;
    delete pInfo;
}


void* GP_Contents_Get(GPContents* contents, int n)
{
    if (contents==NULL || n<0 || n>=contents->size())
    {
        FUNC_PRINT(0);
        return NULL;
    }
    return contents->get(n);
}

GPContents* GP_Contents_CreateCollector()
{
    return new GPContents;
}

void GP_Contents_Collect(GPContents* Collector, GPContents* B, int n)
{
    GPASSERT(NULL!=B && NULL!=Collector && n>=0 && n<B->size());//FIXME
    Collector->pushContent(B->getContent(n));
}

double GP_Contents_GetDouble(GPContents* contents, int n)
{
    GPASSERT(NULL!=contents && n>=0 && contents->size()>n);//FIXME
    return *(double*)(contents->get(n));
}

void GP_Contents_SetDouble(GPContents* contents, double value, int n)
{
    GPASSERT(NULL!=contents && n>=0 && contents->size()>n);//FIXME
    *(double*)(contents->get(n)) = value;
}

AGPStrings* GP_Contents_Types(GPContents* contents)
{
    GPASSERT(NULL!=contents);//FIXME
    std::ostringstream types;
    for (int i=0; i<contents->size(); ++i)
    {
        types << contents->getType(i)->name() << " ";
    }
    AGPStrings* s = new AGPStrings;
    s->a.push_back(types.str());
    return s;
}

GPContents* GP_Contents_CreateDouble(double value)
{
    GPContents* content = new GPContents;
    double* v = new double;
    *v = value;
    content->push(v, gDefaultDoubleType);
    return content;
}

GPContents* GP_Contents_CreateString(const char* value)
{
    GPContents* content = new GPContents;
    std::string* v = new std::string(value);
    content->push(v, gDefaultStringType);
    return content;
}

AGPStrings* GP_Contents_Dump(GPContents* contents, int n)
{
    AGPStrings* s = new AGPStrings;
    if (contents==NULL || n<0 || n>=contents->size())
    {
        FUNC_PRINT(0);
        return s;
    }
    std::ostringstream ostringstream;
    GPPtr<GPWStreamWrap> wrap = GPStreamFactory::NewWStreamFromStl(ostringstream);
    auto c = contents->getContent(n);
    c->type()->vSave(c->content(), wrap.get());
    wrap->vFlush();
    s->a.push_back(ostringstream.str());
    return s;
}

void GP_Function_MapParameters(IGPAutoDefFunction* f, const char* parameters)
{
    if (NULL == parameters)
    {
        FUNC_PRINT(1);
        return;
    }
    std::vector<double> values;
    double v;
    std::istringstream input(parameters);
    while (input >> v)
    {
        values.push_back(v);
    }
    if (values.empty())
    {
        FUNC_PRINT(1);
        return;
    }
    GPPtr<GPParameter> param = new GPParameter((int)values.size());
    auto p = param->attach();
    for (int i=0; i<values.size(); ++i)
    {
        p[i] = values[i];
    }
    f->vMap(param.get());
    f->setParameters(param);
}

GPOptimizorInfo::GPOptimizorInfo()
{
    nMaxADFDepth = 0;
    nOptimizeType = 0;
    pFitComputeFunction = NULL;
    pMeta = NULL;
    nMaxRunTimes = 2000;
    fTargetBestValue = 0.0f;
    pBestInfo = NULL;
    fOutputBest = 0.0f;
}

GPOptimizorInfo::GPOptimizorInfo(const GPOptimizorInfo& info)
{
    ::memcpy(this, &info, sizeof(GPOptimizorInfo));
}
GPOptimizorInfo::~GPOptimizorInfo()
{
}
double GP_OptimzorInfo_TemplateGetBestValue(GPOptimizorInfo* pInfo)
{
    GPASSERT(NULL!=pInfo);//FIXME
    return pInfo->fOutputBest;
}
AGPPiecesProducer* GP_PiecesProducer_Create(AGPProducer* producer, GPStream** piecesLibMeta, IFunctionTable** piecesLibTable, int libNumber, GPStream** mapReduceMeta, int mapReduceMetaNumber)
{
    if (NULL == producer)
    {
        FUNC_PRINT(1);
        return NULL;
    }
    if (NULL == piecesLibMeta || libNumber <= 0)
    {
        FUNC_PRINT(1);
        return NULL;
    }
    if (NULL == mapReduceMeta || mapReduceMetaNumber <= 0)
    {
        FUNC_PRINT(1);
        return NULL;
    }
    GPParallelMachineSet* set = new GPParallelMachineSet();
    for (int i=0; i<libNumber; ++i)
    {
        xmlReader reader;
        auto root = reader.loadStream(piecesLibMeta[i]);
        if (NULL == piecesLibTable)
        {
            set->addFunction(root, NULL);
        }
        else
        {
            set->addFunction(root, piecesLibTable[i]);
        }
    }
    std::map<std::string, std::string> rules;
    for (int i=0; i<mapReduceMetaNumber; ++i)
    {
        xmlReader r;
        auto root = r.loadStream(mapReduceMeta[i]);
        GPASSERT(NULL!=root);
        for (auto p : root->getChildren())
        {
            rules.insert(std::make_pair(p->name(), p->attr()));
        }
    }
    GPPiecesFunctionCreator* creator = GPFactory::createPieceFunctionProducer(producer->P, producer->F, rules);
    
    AGPPiecesProducer* pproducer = new AGPPiecesProducer(set, producer, creator);
    set->decRef();
    creator->decRef();
    return pproducer;
}

void GP_PiecesProducer_Destroy(AGPPiecesProducer* producer)
{
    GPASSERT(NULL!=producer);
    producer->decRef();
}

AGPStrings* GP_PiecesProducer_ListType(AGPPiecesProducer* producer)
{
    AGPStrings* result = new AGPStrings;
    auto names = producer->get()->listAllMachines();
    for (auto s : names)
    {
        result->a.push_back(s);
    }
    return result;
}

GPPiecesFunction* GP_PiecesFunction_Create(AGPPiecesProducer* producer, const char* formula, const char* inputType, const char* type)
{
    if (NULL == producer || NULL == formula || NULL == inputType || NULL == type)
    {
        FUNC_PRINT(1);
        return NULL;
    }
    IParallelMachine* machine = producer->get()->newMachine(type);
    GPPtr<GPFunctionTree> tree = producer->getProducer()->P->getFront()->vCreateFromFormula(formula, _transform(inputType, producer->getProducer()));
    GPPiecesFunction* function = producer->getCreator()->vCreateFromFuncTree(tree.get(), machine);
    delete machine;
    return function;
}

void GP_PiecesFunction_Destroy(GPPiecesFunction* pieceFunction)
{
    GPASSERT(NULL!=pieceFunction);
    pieceFunction->decRef();
}
GPPieces* GP_PiecesFunction_Run(GPPiecesFunction* piecesFunction, GPPieces** inputs, int inputNumber)
{
    if (NULL == piecesFunction || NULL == inputs || 0>= inputNumber)
    {
        FUNC_PRINT(1);
        return NULL;
    }
    return piecesFunction->vRun(inputs, inputNumber);
}

GPPieces* GP_Pieces_Load(AGPPiecesProducer* producer, const char* type, const char* path, const char* description)
{
    GPASSERT(0);
    return NULL;
}

void GP_Pieces_Save(GPPieces* pieces, const char* path, const char* description)
{
    GPASSERT(0);
}

void GP_Pieces_Destroy(GPPieces* pieces)
{
    pieces->decRef();
}

GPPieces* GP_Pieces_CreateInMemory(unsigned int* dimensions, int n)
{
    if (NULL == dimensions || n <= 0)
    {
        FUNC_PRINT(1);
        return NULL;
    }
    std::vector<unsigned int> dims;
    for (int i=0; i<n; ++i)
    {
        dims.push_back(dimensions[i]);
    }
    return GPPieceFactory::createMemoryPiece(dims);
}
