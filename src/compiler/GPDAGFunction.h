/******************************************************************
   Copyright 2016, Jiang Xiao-tang

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
#ifndef COMPILER_GPDAGFUNCTION_H
#define COMPILER_GPDAGFUNCTION_H
#include <stdio.h>
#include "compiler/IGPCompilerInterface.h"
#include "backend/GPComputePoint.h"
class GPDAGFunction : public IGPFunction
{
public:
    virtual GPContents* vRun(GPContents* inputs) override;

    GPDAGFunction(const GP__DAG* dag, const GPFunctionDataBase* database);
    virtual ~GPDAGFunction();

    typedef GPPtr<GPComputePoint::ContentWrap> CONTENT;

    
private:
    class Point:public GPRefCount
    {
    public:
        Point(size_t inputNumber, size_t outputNumber):mOutputs(outputNumber, NULL), mInputs(inputNumber, NULL){}
        virtual ~Point(){}
        virtual bool vReceive(CONTENT c, const Point* source) = 0;
        
        void connectInput(const Point* input, int pos=-1)
        {
            GPASSERT(pos>=0);
            if (pos >= mInputs.size())
            {
                mInputs.resize(pos+1);
            }
            mInputs[pos] = input;
        }
        void connectOutput(GPPtr<Point> p, int pos=-1)
        {
            GPASSERT(pos>=0);
            if (pos >= mOutputs.size())
            {
                mOutputs.resize(pos+1);
            }
            mOutputs[pos] = p;
        }
        
    protected:
        std::vector<GPPtr<Point> > mOutputs;
        std::vector<const Point*> mInputs;
    };
    /*Source Point*/
    class SP:public Point
    {
    public:
        SP():Point(0, 1){}
        virtual ~SP(){}
        virtual bool vReceive(CONTENT con, const Point* source);
    };
    /*Compute Point*/
    class CP:public Point
    {
    public:
        CP(GPPtr<GPComputePoint> point):mPoint(point), Point(point->get()->inputType.size(), point->get()->outputType.size()){mOpen = false;}
        virtual ~CP(){}
        inline GPComputePoint* get() const {return mPoint.get();}
        virtual bool vReceive(CONTENT c, const Point* source);
        void open() {mOpen = true;}
    private:
        GPPtr<GPComputePoint> mPoint;
        bool mOpen;
    };
    /*Transmit Point, Used to copy contents*/
    class TP:public Point
    {
    public:
        TP(int outputNumber=1):Point(1, outputNumber){}
        virtual ~TP(){}
        virtual bool vReceive(CONTENT c, const Point* source);
    };
    /*Dest Point*/
    class DP:public Point
    {
    public:
        DP();
        virtual ~DP() {}
        inline CONTENT get() {return mContents;}
        void reset();
        virtual bool vReceive(CONTENT c, const Point* source);
    private:
        CONTENT mContents;
    };
    
    std::vector<GPPtr<Point>> mSources;
    std::vector<GPPtr<Point>> mDest;

    std::vector<CP*> mFunctions;
};


#endif
