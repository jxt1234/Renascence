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
#include "core/GPProducer.h"
#include "core/GPFactory.h"
#include "core/GPPieceFactory.h"
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

AGPProducer* GP_Producer_Create(GPStream** metaStream, IFunctionTable** table, int n)
{
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
    GPProducer* p = GPFactory::createProducer(f);
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

IGPFunction* GP_Function_Create_ByFormula(const AGPProducer* p, const char* formula, GPOptimizorInfo* pInfo)
{
    if (NULL == formula || NULL == p)
    {
        FUNC_PRINT(1);
        return NULL;;
    }
    return p->P->vCreateContentFunction(formula);
    
    //TODO Complete Optimization
}

GPContents* GP_Function_Run(IGPFunction* f, GPContents* input)
{
    GPASSERT(NULL!=f);
    if(NULL == input)
    {
        GPContents empty;
        return f->vRun(&empty);
    }
    return f->vRun(input);
}

void GP_Function_Destroy(IGPFunction* f)
{
    if (NULL!=f)
    {
        f->decRef();
    }
}

void GP_Function_Optimize(IGPFunction* origin, GPOptimizorInfo* pInfo)
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
    auto paramterSize = origin->vMapParameters(NULL, 0);
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
        origin->vMapParameters(para->get(), para->size());
        return pInfo->pFitComputeFunction(origin, pInfo->pMeta);
    };
    GPPtr<GPParameter> result = opt->vFindBest(paramterSize, optfun);
    origin->vMapParameters(result->get(), result->size());
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
    if (NULL == content)
    {
        FUNC_PRINT(1);
        return;
    }
    content->decRef();
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
    IGPFunction* pPostFunction;
    GPContents* pPostExtraInput;
};


static double _FitValue(IGPFunction* adf, void* pMeta)
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
static double _FitTime(IGPFunction* adf, void* pMeta)
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


GPOptimizorInfo* GP_OptimzorInfo_CreateTemplate(int depth, int maxtimes, int type, GPContents* pInput, GPWStream* bestCache, IGPFunction* pPostFunction, GPContents* pPostExtraInput)
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
    GPPtr<GPWStream> wrap = GPStreamFactory::NewWStreamFromStl(ostringstream);
    auto c = contents->getContent(n);
    c->type()->vSave(c->content(), wrap.get());
    wrap->vFlush();
    s->a.push_back(ostringstream.str());
    return s;
}

AGPStrings* GP_Function_GetFormula(IGPFunction* f, const char* adfName)
{
    GPASSERT(false);
    //TODO
    return NULL;
}

void GP_Function_MapParameters(IGPFunction* f, const char* parameters)
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
    f->vMapParameters(values.data(), values.size());
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
    AGPPiecesProducer* pproducer = new AGPPiecesProducer(set, producer);
    set->decRef();
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


GPPieces* GP_Pieces_Create(AGPPiecesProducer* producer, const char* type, const char* dataType, const char* path, unsigned int* keys, int keyNum, int usage)
{
    if (NULL == producer || NULL == type)
    {
        FUNC_PRINT(1);
        return NULL;
    }
    if (NULL == path && GP_PIECES_CACHE != usage)
    {
        FUNC_PRINT(1);
        return NULL;
    }
    if ((0 == keyNum || NULL == keys) && GP_PIECES_OUTPUT != usage)
    {
        FUNC_PRINT(1);
        return NULL;
    }
    if (GP_PIECES_INPUT == usage && (NULL == dataType))
    {
        FUNC_PRINT(1);
        return NULL;
    }
    const IParallelMachine* machine = producer->get()->getMachine(type);
    if (NULL == machine)
    {
        FUNC_PRINT(1);
        return NULL;
    }
    std::vector<const IStatusType*> inputs;
    if (NULL != dataType)
    {
        inputs = _transform(dataType, producer->getProducer());
    }
    GPASSERT(inputs.size()==1);
    switch (usage)
    {
        case GP_PIECES_INPUT:
            return machine->vCreatePieces(path, inputs[0], keys, keyNum, IParallelMachine::INPUT);
        case GP_PIECES_OUTPUT:
            return machine->vCreatePieces(path, inputs[0], NULL, 0, IParallelMachine::OUTPUT);
        case GP_PIECES_CACHE:
            return machine->vCreatePieces(path, inputs[0], keys, keyNum, IParallelMachine::CACHE);
    }
    FUNC_PRINT(1);
    return NULL;
}

void GP_Pieces_Destroy(GPPieces* pieces)
{
    pieces->decRef();
}

bool GP_Pieces_Copy(AGPPiecesProducer* producer, const char* type, GPPieces* readPieces, GPPieces* writePieces)
{
    if (NULL == producer || NULL == type)
    {
        FUNC_PRINT(1);
        return false;
    }
    const IParallelMachine* machine = producer->get()->getMachine(type);
    if (NULL == machine)
    {
        FUNC_PRINT(1);
        return false;
    }
    return machine->vCopyPieces(readPieces, writePieces);
}

GPPieces** GP_Pieces_CreateArray(int n)
{
    if (n <= 0)
    {
        FUNC_PRINT(1);
        return NULL;
    }
    GPPieces** pieces = (GPPieces**)::malloc(n*sizeof(GPPieces*));
    for (int i=0; i<n; ++i)
    {
        pieces[i] = NULL;
    }
    return pieces;
}
void GP_Pieces_Array_Free(GPPieces** array)
{
    if (NULL == array)
    {
        FUNC_PRINT(1);
        return;
    }
    ::free(array);
}
GPPieces* GP_Pieces_Array_Get(GPPieces** array, int n)
{
    if (n<0 || NULL == array)
    {
        FUNC_PRINT(1);
        return NULL;
    }
    return array[n];
}

void GP_Pieces_Array_Set(GPPieces** array, GPPieces* contents, int n)
{
    if (NULL == array || NULL == contents || n<0)
    {
        FUNC_PRINT(1);
        return;
    }
    array[n] = contents;
}

unsigned int* GP_Unsigned_Int_Array_Create(int n)
{
    return new unsigned int[n];
}
void GP_Unsigned_Int_Array_Free(unsigned int* array)
{
    delete [] array;
}
unsigned int GP_Unsigned_Int_Array_Get(unsigned int* array, int n)
{
    return array[n];
}
void GP_Unsigned_Int_Array_Set(unsigned int* array, unsigned int s, int n)
{
    array[n] = s;
}

IGPPiecesFunction* GP_PiecesFunction_Create(AGPPiecesProducer* producer, const char* formula, const char* inputType, const char* type)
{
    return NULL;
}

bool GP_PiecesFunction_Run(IGPPiecesFunction* piecesFunction, GPPieces** inputs, int inputNumber, GPPieces** output, int outputNumber, GPContents* paramters)
{
    GPASSERT(NULL!=piecesFunction);
    return piecesFunction->vRun(output, outputNumber, inputs, inputNumber, paramters);
}

void GP_PiecesFunction_Destroy(IGPPiecesFunction* pieceFunction)
{
    GPASSERT(NULL!=pieceFunction);
    delete pieceFunction;
}
