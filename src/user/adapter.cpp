#include "core/GPProducer.h"
#include "core/GPStreamFactory.h"
#include "user/GPAPI.h"
#include "AGPProducer.h"
#include <sstream>
#include <fstream>
using namespace std;

extern "C"{
#include "user/adapter.h"
};

void* GO_GP_Producer_Create(const char* metadataname, int type)
{
    /*FIXME*/
    GPASSERT(NULL!=metadataname);
    GPPtr<GPStreamWrap> input = GPStreamFactory::NewStream(metadataname, GPStreamFactory::FILE);
    return (void*)GP_Producer_Create(input.get(), NULL, type);
}
void GO_GP_Producer_Destroy(void* p)
{
    GP_Producer_Destroy((AGPProducer*)p);
}
void* GO_GP_Function_Create_ByType(void* p, const char* outputTypes, const char* inputTypes, int inputRepeat)
{
    return (void*)GP_Function_Create_ByType((AGPProducer*)p, outputTypes, inputTypes, inputRepeat);
}
/*The files contents n file name, which is the same as types number*/
void* GO_GP_CreateContentsByTypes(void* vp, const char* types, const char* files)
{
    /*TODO*/
    return NULL;
#if 0
    /*FIXME*/
    AGPProducer* p = (AGPProducer*)vp;
    GPASSERT(NULL!=files);
    GPASSERT(NULL!=types);
    GPContents* result = new GPContents;
    string type;
    istringstream alltypes(types);
    istringstream allfile(files);
    while(alltypes >> type)
    {
        const IStatusType* t = p->F->vQueryType(type);
        if (NULL != t)
        {
            string fn;
            allfile >> fn;
            ifstream typefile(fn.c_str());
            result->push(t->vLoad(typefile), t);
        }
    }
    return (void*)result;
#endif
}

void* GO_GP_Function_Run(void* f, void* input)
{
    return (void*)GP_Function_Run((IGPAutoDefFunction*)f, (GPContents*)input);
}
void* GO_GP_Function_Create_ByFile(void* vp, const char* file)
{
    GPPtr<GPStreamWrap> inputs = GPStreamFactory::NewStream(file, GPStreamFactory::FILE);
    AGPProducer* p = (AGPProducer*)vp;
    return (void*)GP_Function_Create_ByStream(p, inputs.get());
}
void GO_GP_Function_Save(void* vf, const char* file)
{
    IGPAutoDefFunction* f = (IGPAutoDefFunction*)vf;
    GPPtr<GPWStreamWrap> output = GPStreamFactory::NewWStream(file, GPStreamFactory::FILE);
    GP_Function_Save(f, output.get());
}

