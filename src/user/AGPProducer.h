/*Warp for GPFunctionDataBase and GPProducer*/
class AGPProducer:public RefCount
{
    public:
        AGPProducer(GPFunctionDataBase* f, GPProducer* p)
        {
            GPASSERT(NULL!=f);
            GPASSERT(NULL!=p);
            f->addRef();
            p->addRef();
            P=p;
            F=f;
        }
        virtual ~AGPProducer()
        {
            P->decRef();
            F->decRef();
        }
        GPProducer* P;
        GPFunctionDataBase* F;
};


