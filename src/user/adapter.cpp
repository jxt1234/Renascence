#include "core/GPProducer.h"
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
    ifstream input(metadataname);
    return (void*)GP_Producer_Create(input, NULL, type);
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
}

void* GO_GP_Function_Run(void* f, void* input)
{
    return (void*)GP_Function_Run((IGPAutoDefFunction*)f, (GPContents*)input);
}
void* GO_GP_Function_Create_ByFile(void* vp, const char* file)
{
    ifstream inputs(file);
    AGPProducer* p = (AGPProducer*)vp;
    return (void*)GP_Function_Create_ByStream(p, inputs);
}
void GO_GP_Function_Save(void* vf, const char* file)
{
    IGPAutoDefFunction* f = (IGPAutoDefFunction*)vf;
    ofstream output(file);
    GP_Function_Save(f, output);
}

