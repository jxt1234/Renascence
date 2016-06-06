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
#include "platform/system_lib.h"
using namespace std;


static const char* gFormula =
"REDUCE((MAP((x0), S(x0), [a0]->[a0]), MAP((x1), S(x0), [a0]->[a0])), C(C(x0, y0), x1), [a0, b0]->[1], a0==b0)";

static GPPieces* _createInputPieces(const IStatusType* s)
{
    GPPieces* inputs = GPPieceFactory::createMemoryPiece(std::vector<unsigned int>{5});
    for (int i=1; i<=5; ++i)
    {
        unsigned int key = i-1;
        std::ostringstream os;
        os << "res/pictures/"<<i<<".jpg";
        
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
        os << "output/GPBasicParalleCreator"<<prefix << "_"<<i+1<<".jpg";
        GPPtr<GPWStreamWrap> outputStream = GPStreamFactory::NewWStream(os.str().c_str());
        c->getType(0)->vSave(c->get(0), outputStream.get());
        c->decRef();
    }
}


static void test_main()
{
    GPPtr<GPFunctionDataBase> base = GPFactory::createDataBase("func.xml", NULL);
    {
        GPPtr<GPProducer> totalProducer = GPFactory::createProducer(base.get());
        GPPtr<GPFunctionFrontEndProducer> producer = new GPFunctionFrontEndProducer(base.get());
        GPPtr<GPFunctionTree> tree = producer->vCreateFromFormula(gFormula, std::vector<const IStatusType*>());
        //FUNC_PRINT_ALL(tree->dump().c_str(), s);
        GPSingleParallelMachine machine;
        GPBasicPiecesFunctionCreator creator(totalProducer.get());
        auto function = creator.vCreateFromFuncTree(tree.get(), &machine);
        GPPieces* inputs = _createInputPieces(base->vQueryType("TrBmp"));
        GPPieces* inputAll[2];
        inputAll[0] = inputs;
        inputAll[1] = inputs;
        GPPieces* outputs = function->vRun(inputAll, 2);
        _saveOutputPieces(outputs, "Compose");
        inputs->decRef();
        outputs->decRef();
        
        delete function;
    }
}


#include "user/GPAPI.h"
static string gPath = "/Users/jiangxiaotang/Documents/Renascence/";

int main()
{
    GP_Set_Lib_Path(gPath.c_str());
    GP_Set_Stream_Path(gPath.c_str());
    test_main();
    return 1;
}
