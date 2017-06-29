#include "test/GPTest.h"
#include "compiler/GPSingleParallelMachine.h"
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
    GPPtr<GPFunctionDataBase> base = GPFactory::createDataBase("func.xml", NULL);
    {
        GPPtr<GPProducer> sys = GPFactory::createProducer(base.get());
        GPSingleParallelMachine machine;
        auto bmpType = base->vQueryType("TrBmp");
        {
            GPParallelType data;
            unsigned int keySize = 5;
            GPPieces* output = machine.vCreatePieces("output/GPSingleParallelMachineTest_Map", bmpType, &keySize, 1, IParallelMachine::OUTPUT);
            data.mOutputKey.push_back(std::make_pair(0, 0));
            data.sFuncInfo.formula = "{y0=S(x0);}";
            data.sFuncInfo.variableKey.push_back(std::make_pair(0, 0));
            auto p = machine.vPrepare(&data, IParallelMachine::MAP, sys.get());
            GPPieces* inputs = machine.vCreatePieces("res/pictures/", bmpType, &keySize, 1, IParallelMachine::INPUT);
            GPContents parameter;
            p(&output,1, &inputs, 1, &parameter);

            delete output;
            delete inputs;
        }
        {
            unsigned int keySize = 1;
            GPPieces* output = machine.vCreatePieces("output/GPSingleParallelMachineTest_Reduce",bmpType  , &keySize, 1, IParallelMachine::OUTPUT);
            GPParallelType data;
            data.sFuncInfo.variableKey.push_back(std::make_pair(0, 0));
            data.sFuncInfo.variableKey.push_back(std::make_pair(1, 0));
            data.mReduceInitKey.push_back(std::make_pair(0, 0));
            data.sFuncInfo.formula = "{y0=C(x0, x1);}";
            auto p = machine.vPrepare(&data, IParallelMachine::REDUCE, sys.get());
            keySize = 5;
            GPPieces* inputs = machine.vCreatePieces("res/pictures/", bmpType, &keySize, 1, IParallelMachine::INPUT);
            GPContents parameter;
            p(&output, 1,  &inputs, 1, &parameter);
            delete inputs;
            delete output;
        }
    }
}

void GPSingleParallelMachineTest::run()
{
    __run();
}


static GPTestRegister<GPSingleParallelMachineTest> a("GPSingleParallelMachineTest");
