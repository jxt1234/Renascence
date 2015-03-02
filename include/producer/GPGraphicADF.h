/******************************************************************
   Copyright 2014, Jiang Xiao-tang

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
******************************************************************/
#ifndef PRODUCER_GPGRAPHICADF_H
#define PRODUCER_GPGRAPHICADF_H
#include <stdlib.h>
#include <set>
#include <map>
#include "core/GPStatusContent.h"
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

        virtual int vMap(GPPtr<GPParameter> p);
        /*Return all inputTypes in order*/
        virtual std::vector<const IStatusType*> vGetInputs() const;
        /*Return all outputTypes in order*/
        virtual std::vector<const IStatusType*> vGetOutputs() const;
        GPGraphicADF();
        GPGraphicADF(std::istream& is, const GPFunctionDataBase* base);
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
                Unit(void* c)
                {
                    mC = c;
                    mF = NULL;
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
                static int numberOfInstance();
                Point(const GPFunctionDataBase::function* f, const IStatusType* t);
                void initStatus(const std::vector<std::istream*>& statusInput);
                virtual ~Point();
                void connectInput(Point* in);
                void connectOutput(GPPtr<Point> out);
                /*reset recursive, however, it's not necessary if user choose to clean input in the calling of flow API*/
                void reset() {_reset();}
                /*The owner of output will turn to the user, which means user must free them itself*/
                UNIT collect();
                /*If all inputs is ready, compute and send to next points, otherwise return false*/
                /*clean means every point will clean inputs if when compute done*/
                bool flow(bool clean=true) {return _flow(clean);}
                bool ready() const;
                void receive(GPPtr<Unit> u, Point* source);
            private:
                void _reset();
                bool _flow(bool clean);
                void _clean();
                /*Data Begin*/
                const IStatusType* mType;//For Input/output point, mType and mFunc should only one be not null
                const GPFunctionDataBase::function* mFunc;
                /*The inputs must not be reference for avoiding recursive reference*/
                std::vector<std::pair<Point*, GPPtr<Unit> > > mInputs;
                /*reference the outputs, when clear, they will be released*/
                std::vector<GPPtr<Point> > mOutputs;
                std::vector<GPPtr<GPStatusContent> > mStatus;
                /*Data End*/
                static int gNumber;//FIXME for debug
                friend class GPGraphicADF;
        };
        typedef std::vector<std::pair<Point*, GPPtr<Unit> > >::iterator INPUT_ITER;
        typedef std::vector<std::pair<Point*, GPPtr<Unit> > >::const_iterator INPUT_ITER_CONST;
        typedef std::vector<GPPtr<Point> >::iterator OUTPUT_ITER;
        typedef std::vector<GPPtr<GPStatusContent> >::iterator STATUS_ITER;
    private:
        void _findAllPoints(std::set<Point*>& allPoints) const;
        void _loadMain(const xmlReader::package* root, std::map<std::string, Point*>& allPoints, const GPFunctionDataBase* base) const;
        void _loadStatus(const xmlReader::package* attach, const GPFunctionDataBase* base, Point* currentPoint) const;
        void _loadInputs(const xmlReader::package* attach, const std::map<std::string, Point*>& allPoints, Point* currentPoint);
        void _loadOutputs(const xmlReader::package* attach, const std::map<std::string, Point*>& allPoints, Point* currentPoint);
        /*Every Point receive only one input*/
        std::vector<GPPtr<Point> > mInputs;
        /*Every Point collect one or more output*/
        std::vector<GPPtr<Point> > mOutputs;
        /*To Caculate inputs and outputs is cost, Cache them firstly*/
        std::vector<const IStatusType*> mCacheInputs;
        std::vector<const IStatusType*> mCacheOutputs;
};
#endif