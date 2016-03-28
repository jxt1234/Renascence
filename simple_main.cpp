#include "backend/GPSingleParallelMachine.h"
#include "core/GPFactory.h"
#include "core/GPProducer.h"
#include "core/GPPieceFactory.h"
#include "core/GPStreamFactory.h"
#include "platform/system_lib.h"
#include <string.h>
#include <sstream>
using namespace std;

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
    GPPtr<GPFunctionDataBase> base = GPFactory::createDataBase("func.xml", NULL);
    {
        GPProducer* sys = GPFactory::createProducer(base.get());
        AUTOCLEAN(sys);
        const IStatusType* bmp = base->vQueryType(string("TrBmp"));
        /*Construct compose function*/
        vector<IGPAutoDefFunction*> funcs;
        vector<const IStatusType*> input(3, bmp);
        vector<const IStatusType*> output(1, bmp);
        auto fs = sys->listAllFunction(output, input);
        GPASSERT(fs.size()>0);
        GPContents inp;
        GPPtr<GPStreamWrap> inputStream = GPStreamFactory::NewStream("input.jpg");
        inp.push(bmp->vLoad(inputStream.get()), bmp);
        inputStream = GPStreamFactory::NewStream("input_sharp.jpg");
        inp.push(bmp->vLoad(inputStream.get()), bmp);
        inputStream = GPStreamFactory::NewStream("input_test_simple.jpg");
        inp.push(bmp->vLoad(inputStream.get()), bmp);
        std::vector<const IStatusType*> temp_inputs(2, bmp);
        IGPAutoDefFunction* fit = sys->createFunction("FIT(x0, x1)", temp_inputs);
        
        GPContents targetBitmap;
        inputStream = GPStreamFactory::NewStream("input_test_simple.jpg");
        targetBitmap.push(bmp->vLoad(inputStream.get()), bmp);
        inputStream = NULL;
        int sum = 0;
        for (auto f : fs)
        {
            auto bmpoutput = f->vRun(&inp);
            bmpoutput->merge(targetBitmap);
            auto fitoutput = fit->vRun(bmpoutput);
            double* __fit = (double*)fitoutput->get(0);
            FUNC_PRINT_ALL(*__fit, f);
            delete bmpoutput;
            delete fitoutput;
            sum++;
            if (sum >=10)
            {
                break;
            }
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
