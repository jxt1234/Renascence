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
#include "core/GPComputePoint.h"
#include "core/GPFunctionTree.h"

/*Spectial ADF
 * This kind of ADF will change input and status itself
 */
class GPStreamADF:public IGPAutoDefFunction
{
public:
    typedef GPPtr<GPComputePoint::ContentWrap> CONTENT;
    friend class GPStreamADFProducer;

    GPStreamADF(const GPFunctionTree* root);
    GPStreamADF(const GPTreeNode* n, const GPFunctionDataBase* base);
    virtual ~GPStreamADF();
    
    virtual GPContents* vRun(GPContents* inputs);
    virtual GPTreeNode* vSave() const;
    /*Return a new copy of this Function*/
    virtual IGPAutoDefFunction* vCopy() const;
    /*Return the number of parameter needed, do nothing if para.get()==null*/
    virtual int vMap(GPParameter* para);
    
private:
    class Point:public RefCount
    {
    public:
        Point(){}
        virtual ~Point(){}
        virtual bool vReceive(CONTENT c, const Point* source) = 0;
        
        void connectInput(const Point* input){mInputs.push_back(input);}
        void connectOutput(GPPtr<Point> p){mOutputs.push_back(p);}
        
    protected:
        std::vector<GPPtr<Point> > mOutputs;
        std::vector<const Point*> mInputs;
    };
    /*Source Point*/
    class SP:public Point
    {
    public:
        SP(const IStatusType* type):mType(type){}
        virtual ~SP(){}
        inline const IStatusType* type() const { return mType;}
        inline void setType(const IStatusType* type) {mType = type;}
        virtual bool vReceive(CONTENT con, const Point* source);
    private:
        const IStatusType* mType;
    };
    /*Compute Point*/
    class CP:public Point
    {
    public:
        CP(GPPtr<GPComputePoint> point):mPoint(point){}
        virtual ~CP(){}
        inline GPComputePoint* get() const {return mPoint.get();}
        virtual bool vReceive(CONTENT c, const Point* source);
        GPPtr<GPTreeNode> dump() const;
    private:
        GPPtr<GPComputePoint> mPoint;
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
    
    /*Only has soft reference, for vMap to use*/
    std::vector<CP*> mFunctions;
    
};
#endif /* defined(__GP__GPStreamADF__) */
