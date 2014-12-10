#include "evolution/GPSingleOptimizor.h"
GPPtr<IGPAutoDefFunction> GPSingleOptimizor::vFindBest(GPPtr<IGPAutoDefFunction> origin) const
{
    GPPtr<IGPAutoDefFunction> result = origin->copy();
    PFLOAT before = mFit->compute(result.get());
    int n = result->vMap(NULL);
    if (n <= 0)
    {
        return result;
    }
    class simplecomputer:public IGPOptimizor::IComputer
    {
        public:
            virtual PFLOAT run(GPPtr<GPParameter> p)
            {
                mF->vMap(p);
                return mFit->compute(mF);
            }
            simplecomputer(IGPAutoDefFunction* f, const IGPFitComputer* fit):mF(f), mFit(fit){}
            ~simplecomputer(){}
        private:
            IGPAutoDefFunction* mF;
            const IGPFitComputer* mFit;
    };
    GPPtr<IGPOptimizor::IComputer> c = new simplecomputer(result.get(), mFit.get());
    GPPtr<GPParameter> _p = mOpt->vFindBest(n, c);
    result->vMap(_p);
    PFLOAT after = mFit->compute(result.get());
    if (after < before)
    {
        /*Optimize failed Turn back to origin*/
        result = origin->copy();
    }
    return result;
}

GPSingleOptimizor::GPSingleOptimizor(GPPtr<IGPFitComputer> fit, GPOptimizorFactory::TYPE t)
{
    mFit = fit;
    mOpt = GPOptimizorFactory::create(t);
}

GPSingleOptimizor::~GPSingleOptimizor()
{
}

class psoOptimizorcreator:public IGPADFOptimizorCreator
{
    public:
        virtual GPPtr<IGPADFOptimizor> vCreate(const GPProducer* producer, GPPtr<IGPFitComputer> fit) const
        {
            return new GPSingleOptimizor(fit, GPOptimizorFactory::PSO_SEARCH);
        }
};

static GPADFOptimizorRegistor<psoOptimizorcreator> _pso("PSO_Single", "Use PSO to optimize single function");

class goldencreator:public IGPADFOptimizorCreator
{
    public:
        virtual GPPtr<IGPADFOptimizor> vCreate(const GPProducer* producer, GPPtr<IGPFitComputer> fit) const
        {
            return new GPSingleOptimizor(fit, GPOptimizorFactory::GOLDEN_DIVIDE);
        }
};

static GPADFOptimizorRegistor<goldencreator> _golden("Golden_Single", "Golden divide method");
