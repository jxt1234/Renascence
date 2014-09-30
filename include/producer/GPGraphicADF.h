#include <stdlib.h>
#include "core/GPFunctionDataBase.h"
#include "core/IGPAutoDefFunction.h"
#include "utils/GP_Clock.h"
#include "utils/debug.h"

/*Single Graphic ADF, Input to the source node and then collect outputs from outputNodes*/
class GPGraphicADF:public IGPAutoDefFunction
{
    public:
        typedef GP_Output::GP_Unit UNIT;
        virtual GP_Output run(const GP_Input& inputs);
        virtual void save(std::ostream& os) const;
        /*Return a new copy of this Function*/
        virtual IGPAutoDefFunction* copy() const;
        /*Return all inputTypes in order*/
        virtual std::vector<const IStatusType*> vGetInputs() const;
        /*Return all outputTypes in order*/
        virtual std::vector<const IStatusType*> vGetOutputs() const;

        GPGraphicADF();
        virtual ~GPGraphicADF();

        class Unit:public RefCount
        {
            public:
                void* content() const {return mC;}
                UNIT warp()
                {
                    UNIT u;
                    u.content = mC;
                    u.freeCallBack = mF;

                    mF = NULL;
                    return u;
                }
                Unit(const UNIT& u)
                {
                    mC = u.content;
                    mF = u.freeCallBack;
                }
                virtual ~Unit()
                {
                    if(NULL!=mF)
                    {
                        mF(mC);
                    }
                    mC = NULL;
                    mF = NULL;
                }
            private:
                void* mC;
                void(*mF)(void*);
        };


        /*Each Point only send one result to another Point*/
        class Point:public RefCount
        {
            public:
                Point(const GPFunctionDataBase::function* f);
                virtual ~Point();
                void connectInput(GPPtr<Point> in);
                void connectOutput(GPPtr<Point> out);
                /*reset recursive, however, it's not necessary if user choose to clean input in the calling of flow API*/
                void reset() {_reset();}
                /*The owner of output will turn to the user, which means user must free them itself*/
                GP_Output collect();
                /*If all inputs is ready, compute and send to next points, otherwise return false*/
                /*clean means every point will clean inputs if when compute done*/
                bool flow(bool clean=true) {return _flow(clean);}
                bool ready() const;
                void receive(GPPtr<Unit> u, Point* source);
            private:
                void _reset();
                bool _flow(bool clean);
                void _clean();
                const GPFunctionDataBase::function* mFunc;
                /*The inputs must not be reference for avoiding recursive reference*/
                std::vector<std::pair<Point*, GPPtr<Unit> > > mInputs;
                /*reference the outputs, when clear, they will be released*/
                std::vector<GPPtr<Point> > mOutputs;
                typedef std::vector<std::pair<Point*, GPPtr<Unit> > >::iterator INPUT_ITER;
                typedef std::vector<std::pair<Point*, GPPtr<Unit> > >::const_iterator INPUT_ITER_CONST;
                typedef std::vector<GPPtr<Point> >::iterator OUTPUT_ITER;
        };

    private:
        /*Every Point receive only one input*/
        std::vector<GPPtr<Point> > mInput;
        /*Every Point collect one or more output*/
        std::vector<GPPtr<Point> > mOutput;

        /*To Caculate inputs and outputs is cost, Cache them firstly*/
        std::vector<const IStatusType*> mCacheInputs;
        std::vector<const IStatusType*> mCacheOutputs;
};
