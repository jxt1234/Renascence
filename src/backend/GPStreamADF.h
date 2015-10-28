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

/*Spectial ADF
 * This kind of ADF will change input and status itself
 */
class GPStreamADF:public IGPAutoDefFunction
{
public:
    friend class GPStreamADFProducer;
    GPStreamADF(const GPTreeNode* n, const GPFunctionDataBase* base);
    virtual ~GPStreamADF();
    
    virtual GPContents* vRun(GPContents* inputs);
    virtual GPTreeNode* vSave() const;
    /*Return a new copy of this Function*/
    virtual IGPAutoDefFunction* vCopy() const;
    /*Mutate function, for evolution*/
    virtual int vMapStructure(GPParameter* para, bool& changed);
    /*Return the number of parameter needed, do nothing if para.get()==null*/
    virtual int vMap(GPParameter* para);
    
private:
    class Point:public RefCount
    {
    public:
        Point(){}
        virtual ~Point(){}
        virtual bool vReceive(GPContents* c, const Point* source) = 0;
        
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
        virtual bool vReceive(GPContents* con, const Point* source);
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
        virtual bool vReceive(GPContents* c, const Point* source);
        GPPtr<GPTreeNode> dump() const;
    private:
        GPPtr<GPComputePoint> mPoint;
    };
    /*Dest Point*/
    class DP:public Point
    {
    public:
        DP(const IStatusType* type);
        virtual ~DP() {mContents.clear();}
        inline GPContents& get() {return mContents;}
        inline const IStatusType* getType() const {return mContents.contents[0].type;}
        void reset();
        virtual bool vReceive(GPContents* c, const Point* source);
    private:
        GPContents mContents;
    };
    
    std::vector<GPPtr<SP>> mSources;
    std::vector<GPPtr<DP>> mDest;
    
    /*Only has soft reference, for vMap to use*/
    std::vector<CP*> mFunctions;
    
    GPStreamADF(const std::vector<GPPtr<SP> >& Source, const std::vector<GPPtr<DP>>& dest, const std::vector<CP*>& functions);
};
#endif /* defined(__GP__GPStreamADF__) */
