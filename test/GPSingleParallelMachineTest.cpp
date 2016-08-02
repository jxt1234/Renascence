#include "test/GPTest.h"
#include "backend/GPSingleParallelMachine.h"
#include "core/GPFactory.h"
#include "core/GPProducer.h"
#include "core/GPPieceFactory.h"
#include "core/GPStreamFactory.h"
#include <string.h>
#include <sstream>

class GPSingleParallelMachineTest:public GPTest
{
    public:
        virtual void run();
        GPSingleParallelMachineTest(){}
        virtual ~GPSingleParallelMachineTest(){}
};

static void __run()
{
    GPSingleParallelMachine machine;
    GPPtr<GPFunctionDataBase> base = GPFactory::createDataBase("func.xml", NULL);
    {
        GPPtr<GPProducer> sys = GPFactory::createProducer(base.get());
        {
            GPParallelType data;
            GPPieces* output = machine.vCreatePieces("output/GPSingleParallelMachineTest_Map", std::vector<const IStatusType*>(), NULL, 0, IParallelMachine::OUTPUT);
            data.mOutputKey.push_back(std::make_pair(0, 0));
            data.pContext = sys.get();
            data.sFuncInfo.formula = "S(x0)";
            data.sFuncInfo.variableKey.push_back(std::make_pair(0, 0));
            auto p = machine.vPrepare(&data, IParallelMachine::MAP);
            GPPieces* inputs = GPPieceFactory::createLocalFilePiece(std::vector<const IStatusType*>{base->vQueryType("TrBmp")}, "res/pictures/", 0, false);
            inputs->pKeySize[0] = 5;
            inputs->nKeyNumber = 1;
            output->pKeySize[0] = 5;
            output->nKeyNumber = 1;
            p->vRun(output, &inputs, 1);

            delete output;
            delete inputs;
            delete p;
        }
        {
            GPPieces* output = machine.vCreatePieces("output/GPSingleParallelMachineTest_Reduce", std::vector<const IStatusType*>(), NULL, 0, IParallelMachine::OUTPUT);
            GPParallelType data;
            data.pContext = sys.get();
            data.sFuncInfo.variableKey.push_back(std::make_pair(0, 0));
            data.sFuncInfo.variableKey.push_back(std::make_pair(1, 0));
            data.sFuncInfo.formula = "C(x0, x1)";
            auto p = machine.vPrepare(&data, IParallelMachine::REDUCE);
            GPPieces* inputs = GPPieceFactory::createLocalFilePiece(std::vector<const IStatusType*>{base->vQueryType("TrBmp")}, "res/pictures/", 0, false);
            inputs->pKeySize[0] = 5;
            inputs->nKeyNumber = 1;
            output->pKeySize[0] = 1;
            output->nKeyNumber = 1;
            GPPieces* tempOutput = machine.vCreatePieces(NULL, std::vector<const IStatusType*>(), output->pKeySize, output->nKeyNumber, IParallelMachine::CACHE);
            p->vRun(tempOutput, &inputs, 1);
            machine.vCopyPieces(tempOutput, output);

            delete inputs;
            delete p;
            delete output;
            delete tempOutput;
        }
    }
}

void GPSingleParallelMachineTest::run()
{
    __run();
}


static GPTestRegister<GPSingleParallelMachineTest> a("GPSingleParallelMachineTest");
