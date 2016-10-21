#include "test/GPTest.h"
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

class GPPiecesFunctionFactoryTest:public GPTest
{
    public:
        virtual void run();
        GPPiecesFunctionFactoryTest(){}
        virtual ~GPPiecesFunctionFactoryTest(){}
};

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
        os << "output/GPFactoryPiecesFunctionCreator"<<prefix << "_"<<i+1<<".jpg";
        GPPtr<GPWStream> outputStream = GPStreamFactory::NewWStream(os.str().c_str());
        c->getType(0)->vSave(c->get(0), outputStream.get());
        c->decRef();
    }
}


static void __run()
{
    GPPtr<GPFunctionDataBase> base = GPFactory::createDataBase("func.xml", NULL);
    GPPtr<GPStream> map_reduce = GPStreamFactory::NewStream("Map-Reduce.xml");
    {
        GPPtr<GPProducer> totalProducer = GPFactory::createProducer(base.get());
        GPPtr<GPPiecesFunctionCreator> creator = GPFactory::createPieceFunctionProducer(totalProducer.get(), base.get(), map_reduce.get());
        GPSingleParallelMachine machine;
        GPPtr<GPFunctionTree> tree = totalProducer->getFront()->vCreateFromFormula("C(S(S(x0)))", std::vector<const IStatusType*>());
        auto function = creator->vCreateFromFuncTree(tree.get(), &machine);
        GPPieces* inputs = GPPieceFactory::createLocalFilePiece(std::vector<const IStatusType*>{base->vQueryType("TrBmp")}, "res/pictures/", 0);
        inputs->pKeySize[0] = 5;
        inputs->nKeyNumber = 1;
        GPPieces* outputs = function->vRun(&inputs, 1);
        _saveOutputPieces(outputs, "Compose");
        delete inputs;
        delete outputs;
        
        delete function;
    }
}
void GPPiecesFunctionFactoryTest::run()
{
    __run();
}

static GPTestRegister<GPPiecesFunctionFactoryTest> a("GPPiecesFunctionFactoryTest");
