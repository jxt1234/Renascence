#include "frontend/GPFormulaTree.h"
#include "frontend/GPFunctionFrontEndProducer.h"
#include "backend/GPBasicPiecesFunctionCreator.h"
#include "backend/GPSingleParallelMachine.h"
#include "core/GPFactory.h"
#include "core/GPProducer.h"
#include "core/GPFunctionTree.h"
#include <iostream>
#include <sstream>
#include "core/GPPieceFactory.h"
#include <fstream>
#include "core/GPStreamFactory.h"
#include "core/GPParallelMachineSet.h"
#include "user/GPAPI.h"
using namespace std;


static GPPieces* _createInputPieces(const IStatusType* s)
{
    GPPieces* inputs = GPPieceFactory::createMemoryPiece(std::vector<unsigned int>{50});
    for (int i=1; i<=50; ++i)
    {
        unsigned int key = i-1;
        std::ostringstream os;
        os << "res/pictures/"<<((i%5))+1<<".jpg";
        
        GPPtr<GPStreamWrap> input = GPStreamFactory::NewStream(os.str().c_str());
        GPContents* c = new GPContents;
        c->push(s->vLoad(input.get()), s);
        inputs->vSave(&key, 1, c);
        c->decRef();
    }
    return inputs;
}

static void _saveOutputPieces(GPPieces* output, const char* prefix)
{
    GPASSERT(output->nKeyNumber <= 1);
    unsigned int n = 1;
    if (output->nKeyNumber == 1)
    {
        n = output->pKeySize[0];
    }
    for (int i=0; i<n; ++i)
    {
        unsigned int key = i;
        GPContents* c = output->vLoad(&key, output->nKeyNumber);
        GPASSERT(c->size() == 1);
        std::stringstream os;
        os << "output/GPThreadParallelTest"<<prefix << "_"<<i+1<<".jpg";
        GPPtr<GPWStreamWrap> outputStream = GPStreamFactory::NewWStream(os.str().c_str());
        c->getType(0)->vSave(c->get(0), outputStream.get());
        c->decRef();
    }
}


static void __run()
{
    GPPtr<GPFunctionDataBase> base = GPFactory::createDataBase("func.xml", NULL);
    GPPtr<GPParallelMachineSet> machineSet = GPFactory::createParallelSet("mgpfunc.xml", NULL);
    auto names = machineSet->listAllMachines();
    for (auto name : names)
    {
        FUNC_PRINT_ALL(name.c_str(), s);
    }
    GPPtr<GPStreamWrap> map_reduce = GPStreamFactory::NewStream("Map-Reduce.xml");
    {
        GPPtr<GPProducer> totalProducer = GPFactory::createProducer(base.get());
        GPPtr<GPPiecesFunctionCreator> creator = GPFactory::createPieceFunctionProducer(totalProducer.get(), base.get(), map_reduce.get());
        GPPtr<GPFunctionTree> tree = totalProducer->getFront()->vCreateFromFormula("C(S(x0))", std::vector<const IStatusType*>());
        GPPieces* inputs = _createInputPieces(base->vQueryType("TrBmp"));
        {
            IParallelMachine* machine = machineSet->newMachine("thread");
            GPASSERT(NULL!=machine);
            auto function = creator->vCreateFromFuncTree(tree.get(), machine);
            GPPieces* outputs = NULL;
            {
                GPCLOCK;
                outputs = function->vRun(&inputs, 1);
            }
            _saveOutputPieces(outputs, "Compose");
            delete outputs;
            delete function;
            delete machine;
        }
        {
            IParallelMachine* machine = machineSet->newMachine("basic");
            GPASSERT(NULL!=machine);
            auto function = creator->vCreateFromFuncTree(tree.get(), machine);
            GPPieces* outputs = NULL;
            {
                GPCLOCK;
                outputs = function->vRun(&inputs, 1);
            }
            _saveOutputPieces(outputs, "ComposeBasic");
            delete outputs;
            delete function;
            delete machine;
        }
        delete inputs;
        
    }
}

#include "user/GPAPI.h"
static string gPath = "/Users/jiangxiaotang/Documents/Renascence/";

int main()
{
    GP_Set_Lib_Path(gPath.c_str());
    GP_Set_Stream_Path(gPath.c_str());
    //while(true)
    {
        __run();
    }
    return 1;
}
