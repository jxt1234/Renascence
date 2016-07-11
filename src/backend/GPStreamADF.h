/******************************************************************
 Copyright 2015, Jiang Xiao-tang
 
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

#ifndef __GP__GPStreamADF__
#define __GP__GPStreamADF__

#include "core/IGPAutoDefFunction.h"
#include "core/GPFunctionTree.h"
#include "midend/GPMultiLayerTree.h"
#include "backend/GPComputePoint.h"

/*Spectial ADF
 * This kind of ADF will change input and status itself
 */
class GPStreamADF:public IGPAutoDefFunction
{
public:
    typedef GPPtr<GPComputePoint::ContentWrap> CONTENT;
    friend class GPStreamADFProducer;

    GPStreamADF(const GPMultiLayerTree* opttree);
    GPStreamADF(const GPTreeNode* n, const GPFunctionDataBase* base);
    virtual ~GPStreamADF();
    
    virtual GPContents* vRun(GPContents* inputs);
    virtual GPTreeNode* vSave() const;
    /*Return the number of parameter needed, do nothing if para.get()==null*/
    virtual int vMap(GPParameter* para);
    
private:
    static void _addPoint();
    class Point:public GPRefCount
    {
    public:
        Point(int inputNumber, int outputNumber):mOutputs(outputNumber, NULL), mInputs(inputNumber, NULL){}
        virtual ~Point(){}
        virtual bool vReceive(CONTENT c, const Point* source) = 0;
        
        void connectInput(const Point* input, int pos=-1) {GPASSERT(pos>=0 && pos <mInputs.size()); mInputs[pos] = input;}
        void connectOutput(GPPtr<Point> p, int pos=-1){GPASSERT(pos>=0 && pos <mOutputs.size());mOutputs[pos] = p;}
        
    protected:
        std::vector<GPPtr<Point> > mOutputs;
        std::vector<const Point*> mInputs;
    };
    /*Source Point*/
    class SP:public Point
    {
    public:
        SP(const IStatusType* type):mType(type), Point(0, 1){}
        virtual ~SP(){}
        inline const IStatusType* type() const { return mType;}
        inline void setType(const IStatusType* type)
        {
            if (mType != NULL && type != mType)
            {
                FUNC_PRINT_ALL(type->name().c_str(), s);
                FUNC_PRINT_ALL(mType->name().c_str(), s);
            }
            GPASSERT(NULL==mType || mType==type); mType = type;
        }
        virtual bool vReceive(CONTENT con, const Point* source);
    private:
        const IStatusType* mType;
    };
    /*Compute Point*/
    class CP:public Point
    {
    public:
        CP(GPPtr<GPComputePoint> point):mPoint(point), Point(point->get()->inputType.size(), point->get()->outputType.size()){mOpen = false;}
        virtual ~CP(){}
        inline GPComputePoint* get() const {return mPoint.get();}
        virtual bool vReceive(CONTENT c, const Point* source);
        GPPtr<GPTreeNode> dump() const;
        void open() {mOpen = true;}
    private:
        GPPtr<GPComputePoint> mPoint;
        bool mOpen;
    };
    /*Transmit Point, Used to copy contents*/
    class TP:public Point
    {
    public:
        TP(int outputNumber):Point(1, outputNumber){}
        virtual ~TP(){}
        virtual bool vReceive(CONTENT c, const Point* source);
        GPPtr<GPTreeNode> dump() const;
    };
    /*Dest Point*/
    class DP:public Point
    {
    public:
        DP(const IStatusType* type);
        virtual ~DP() {}
        inline CONTENT get() {return mContents;}
        inline const IStatusType* getType() const {return mType;}
        void reset();
        virtual bool vReceive(CONTENT c, const Point* source);
    private:
        CONTENT mContents;
        const IStatusType* mType;
    };
    
    std::vector<GPPtr<Point>> mSources;
    std::vector<int> mInputPos;
    std::vector<GPPtr<Point>> mDest;
    
    /*Only has soft reference, for vMap and vSave to use*/
    std::vector<CP*> mFunctions;
    /*Only has soft reference, for vSave to use*/
    std::vector<TP*> mTPS;
    
};
#endif /* defined(__GP__GPStreamADF__) */
