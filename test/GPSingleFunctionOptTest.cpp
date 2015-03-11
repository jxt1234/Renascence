#include "test/GPTest.h"
#include "core/GPFactory.h"
#include "core/GPProducer.h"
#include "optimizor/GPOptimizorFactory.h"
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
        GPPtr<IGPAutoDefFunction> func = sys->vCreateFunctionFromName("TrPackageFitCompute");
        GPPtr<IGPOptimizor> opt = GPOptimizorFactory::create(GPOptimizorFactory::PSO_SEARCH);

        GPPtr<IGPAutoDefFunction> origin = sys->vCreateFunction(base->queryType("TrBmp"), base->queryType(""));
        origin->save(std::cout);
        GPPtr<GPParameter> nullp;
        int n = origin->vMap(nullp);
        auto optfunc = [&](GPPtr<GPParameter> p){
            origin->vMap(p);
            GPContents firstinputs;
            GPContents* result = origin->run(&firstinputs);
            GPContents* fit = func->run(result);
            double r = *(double*)fit->get(0);
            result->clear();
            fit->clear();
            delete result;
            delete fit;
            return r;
        };
        GPPtr<GPParameter> p = opt->vFindBest(n, optfunc);
        std::cout << optfunc(p) <<std::endl;
    }
}

static GPTestRegister<GPSingleFunctionOptTest> a("GPSingleFunctionOptTest");
