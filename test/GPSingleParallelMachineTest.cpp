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
        os << "output/GPSingleParallelMachineTest"<<prefix << "_"<<i+1<<".jpg";
        GPPtr<GPWStreamWrap> outputStream = GPStreamFactory::NewWStream(os.str().c_str());
        c->getType(0)->vSave(c->get(0), outputStream.get());
        c->decRef();
    }
}

static void __run()
{
    GPSingleParallelMachine machine;
    GPPtr<GPFunctionDataBase> base = GPFactory::createDataBase("func.xml", NULL);
    {
        GPPtr<GPProducer> sys = GPFactory::createProducer(base.get());
        {
            GPParallelType data;
            data.mOutputKey.push_back(std::make_pair(0, 0));
            data.pContext = sys.get();
            data.sFuncInfo.formula = "S(x0)";
            data.sFuncInfo.variableKey.push_back(std::make_pair(0, 0));
            auto p = machine.vGenerate(&data, IParallelMachine::MAP);
            GPPieces* inputs = GPPieceFactory::createLocalFilePiece(std::vector<const IStatusType*>{base->vQueryType("TrBmp")}, "res/pictures/", 0);
            inputs->pKeySize[0] = 5;
            inputs->nKeyNumber = 1;
            GPPieces* outputs = p.first->vPrepare(&inputs, 1);
            p.second->vRun(outputs, &inputs, 1);
            _saveOutputPieces(outputs, "Map");
            
            delete outputs;
            delete inputs;
            
            delete p.first;
            delete p.second;
        }
        {
            GPParallelType data;
            data.pContext = sys.get();
            data.sFuncInfo.variableKey.push_back(std::make_pair(0, 0));
            data.sFuncInfo.variableKey.push_back(std::make_pair(1, 0));
            data.sFuncInfo.formula = "C(x0, x1)";
            auto p = machine.vGenerate(&data, IParallelMachine::REDUCE);
            GPPieces* inputs = GPPieceFactory::createLocalFilePiece(std::vector<const IStatusType*>{base->vQueryType("TrBmp")}, "res/pictures/", 0);
            inputs->pKeySize[0] = 5;
            inputs->nKeyNumber = 1;
            GPPieces* outputs = p.first->vPrepare(&inputs, 1);
            p.second->vRun(outputs, &inputs, 1);
            _saveOutputPieces(outputs, "Reduce");
            
            delete outputs;
            delete inputs;
            delete p.first;
            delete p.second;
        }
    }
}

void GPSingleParallelMachineTest::run()
{
    __run();
}


static GPTestRegister<GPSingleParallelMachineTest> a("GPSingleParallelMachineTest");
