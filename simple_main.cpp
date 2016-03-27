#include "backend/GPSingleParallelMachine.h"
#include "core/GPFactory.h"
#include "core/GPProducer.h"
#include "core/GPPieceFactory.h"
#include "core/GPStreamFactory.h"
#include "platform/system_lib.h"
#include <string.h>
#include <sstream>

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
        c->push(s->vLoad(input.get()), s, true);
        inputs->save(&key, 1, c);
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
        GPContents* c = output->load(&key, output->nKeyNumber);
        GPASSERT(c->size() == 1);
        std::stringstream os;
        os << "output/GPSingleParallelMachineTest"<<prefix << "_"<<i+1<<".jpg";
        GPPtr<GPWStreamWrap> outputStream = GPStreamFactory::NewWStream(os.str().c_str());
        auto type = c->getType(0);
        auto content = c->get(0);
        type->vSave(content, outputStream.get());
        delete c;
    }
}

static void __run()
{
    GPSingleParallelMachine machine;
    GPPtr<GPFunctionDataBase> base = GPFactory::createDataBase("func.xml", NULL);
    {
        GPParallelType data;
        data.pCondition = NULL;
        GPPtr<GPProducer> sys = GPFactory::createProducer(base.get());
        {
            data.mOutputKey.clear();
            data.mOutputKey.push_back(std::make_pair(0, 0));
            data.pFunc = sys->createFunction("F(F((S(x0))))", std::vector<const IStatusType*>());
            auto p = machine.vGenerate(&data, IParallelMachine::MAP);
            GPPieces* inputs = _createInputPieces(base->vQueryType("TrBmp"));
            GPPieces* outputs = p.first->vPrepare(&inputs, 1);
            p.second->vRun(outputs, &inputs, 1);
            _saveOutputPieces(outputs, "Map");
            
            outputs->pFree(outputs->pMeta);
            delete outputs;
            inputs->pFree(inputs->pMeta);
            delete inputs;
            
            delete p.first;
            delete p.second;
            delete data.pFunc;
        }
        {
            data.mOutputKey.clear();
            data.pFunc = sys->createFunction("C(x0, x1)", std::vector<const IStatusType*>());
            auto p = machine.vGenerate(&data, IParallelMachine::REDUCE);
            GPPieces* inputs = _createInputPieces(base->vQueryType("TrBmp"));
            GPPieces* outputs = p.first->vPrepare(&inputs, 1);
            p.second->vRun(outputs, &inputs, 1);
            _saveOutputPieces(outputs, "Reduce");
            
            outputs->pFree(outputs->pMeta);
            delete outputs;
            
            inputs->pFree(inputs->pMeta);
            delete inputs;
            delete p.first;
            delete p.second;
            delete data.pFunc;
        }
    }
}


static const char* gPath = "/Users/jiangxiaotang/Documents/Renascence/";

int main()
{
    GPStreamFactory::setParentPath(gPath);
    system_set_path(gPath);
    __run();
    return 1;
}
