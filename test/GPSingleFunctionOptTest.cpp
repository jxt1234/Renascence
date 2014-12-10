#include "test/GPTest.h"
#include "core/GPFactory.h"
#include "evolution/GPADFOptimizorFactory.h"
#include "evolution/GPFunctionFitcomputer.h"
#include <iostream>
class GPSingleFunctionOptTest:public GPTest
{
    public:
        virtual void run();
        GPSingleFunctionOptTest(){}
        virtual ~GPSingleFunctionOptTest(){}
};
void GPSingleFunctionOptTest::run()
{
    GPPtr<GPFunctionDataBase> base = GPFactory::createDataBase("func.xml", NULL);
    {
        GPPtr<GPProducer> sys = GPFactory::createProducer(base.get());
        GP_Input emptyInput;
        GPPtr<IGPFitComputer> fitcomputer = new GPFunctionFitcomputer("TrPackageFitCompute", sys.get(), emptyInput);
        GPPtr<IGPADFOptimizor> opt = GPADFOptimizorFactory::create("PSO_Single", sys.get(), fitcomputer);
        GPPtr<IGPAutoDefFunction> origin = sys->vCreateFunction(base->queryType("TrBmp"), base->queryType(""));
        origin->save(std::cout);
        GPPtr<IGPAutoDefFunction> bestfunc = opt->vFindBest(origin);
        std::cout << fitcomputer->compute(bestfunc.get()) <<std::endl;
    }
}

static GPTestRegister<GPSingleFunctionOptTest> a("GPSingleFunctionOptTest");
