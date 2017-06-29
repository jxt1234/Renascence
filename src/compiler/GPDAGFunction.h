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
#include "compiler/GPComputePoint.h"
class GPDAGFunction : public IGPFunction
{
public:
    virtual GPContents* vRun(GPContents* inputs) override;
    virtual size_t vMapParameters(const double* p, size_t n) override;

    GPDAGFunction(const GP__DAG* dag, const GPFunctionDataBase* database);
    virtual ~GPDAGFunction();

    typedef GPPtr<GPComputePoint::ContentWrap> CONTENT;

    
private:
    class Point:public GPRefCount
    {
    public:
        Point(size_t inputNumber, size_t outputNumber){}
        virtual ~Point(){}
        virtual bool vReceive(CONTENT c, int put_dst) = 0;
        
        void connectOutput(GPPtr<Point> p, int get_src, int put_dst)
        {
            GPASSERT(get_src>=0);
            GPASSERT(put_dst>=0);
            mOutputs.push_back(std::make_pair(get_src, std::make_pair(p, put_dst)));
        }
        
    protected:
        std::vector<std::pair<int, std::pair<GPPtr<Point>, int>> > mOutputs;
    };
    /*Source Point*/
    class SP:public Point
    {
    public:
        SP():Point(0, 1){}
        virtual ~SP(){}
        virtual bool vReceive(CONTENT con, int put_dst);
    };
    /*Compute Point*/
    class CP:public Point
    {
    public:
        CP(GPPtr<GPComputePoint> point):mPoint(point), Point(point->get()->inputType.size(), point->get()->outputType.size()){}
        virtual ~CP(){}
        inline GPComputePoint* get() const {return mPoint.get();}
        virtual bool vReceive(CONTENT c, int put_dst);
    private:
        GPPtr<GPComputePoint> mPoint;
    };
    /*Transmit Point, Used to copy contents*/
    class TP:public Point
    {
    public:
        TP(int outputNumber=1):Point(1, outputNumber){}
        virtual ~TP(){}
        virtual bool vReceive(CONTENT c, int put_dst);
    };
    /*Dest Point*/
    class DP:public Point
    {
    public:
        DP();
        virtual ~DP() {}
        inline CONTENT get() {return mContents;}
        void reset();
        virtual bool vReceive(CONTENT c, int put_dst);
    private:
        CONTENT mContents;
    };
    
    std::vector<std::pair<uint32_t, GPPtr<Point>>> mSources;
    std::vector<GPPtr<Point>> mDest;
    
    std::vector<GPComputePoint*> mComputePoints;//For map paramters
};


#endif
