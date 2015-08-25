//
//  GPStreamADF.h
//  GP
//
//  Created by jiangxiaotang on 15/8/25.
//  Copyright (c) 2015年 jiangxiaotang. All rights reserved.
//

#ifndef __GP__GPStreamADF__
#define __GP__GPStreamADF__

#include "core/IGPAutoDefFunction.h"
#include "core/GPComputePoint.h"
class GPStreamADF:public IGPAutoDefFunction
{
public:
    GPStreamADF(const GPTreeNode* n);
    virtual ~GPStreamADF();
    
    virtual GPContents* vRun(GPContents* inputs);
    virtual GPTreeNode* vSave() const;
    /*Return a new copy of this Function*/
    virtual IGPAutoDefFunction* vCopy() const;
    /*Mutate function, for evolution*/
    virtual void vMutate();
    /*Return the number of parameter needed, do nothing if para.get()==null*/
    virtual int vMap(GPPtr<GPParameter> para);
    
private:
    class Point:public RefCount
    {
    public:
        Point(){}
        virtual ~Point();
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
        SP(GPStream* input, const IStatusType* type):mInput(input), mType(type){}
        virtual ~SP(){}
        virtual bool vReceive(GPContents* con, const Point* source)
        {
            void* contents = mType->vLoad(mInput);
            if (NULL == contents)
            {
                return false;
            }
            auto c = new GPContents;
            c->push(contents, mType);
            GPASSERT(1 == mOutputs.size());
            mOutputs[0]->vReceive(c, this);
            delete c;
            return true;
        }
    private:
        GPStream* mInput;
        const IStatusType* mType;
    };
    /*Compute Point*/
    class CP:public Point
    {
    public:
        CP(GPPtr<GPComputePoint> point):mPoint(point){}
        virtual ~CP(){}
        virtual bool vReceive(GPContents* c, const Point* source)
        {
            for (int i=0; i<mInputs.size(); ++i)
            {
                if (mInputs[i] == source)
                {
                    if (mPoint->receiveSingle(c, i))
                    {
                        GPContents* c = mPoint->compute();
                        GPASSERT(c->size() == mOutputs.size());
                        GPContents unit;
                        for (int i=0; i<c->size(); ++i)
                        {
                            unit.push((*c)[i]);
                            mOutputs[i]->vReceive(&unit, this);
                        }
                        /*Shouldn't clear c*/
                        delete c;
                    }
                    return true;
                }
            }
            GPASSERT(0);
            return false;
        }
    private:
        GPPtr<GPComputePoint> mPoint;
    };
    /*Dest Point*/
    class DP:public Point
    {
    public:
        DP(){}
        virtual ~DP() {mContents.clear();}
        inline GPContents& get() {return mContents;}
        void reset()
        {
            mContents.clear();
            for (int i=0; i<mInputs.size(); ++i)
            {
                mContents.push(NULL, NULL);
            }
        }
        virtual bool vReceive(GPContents* c, const Point* source)
        {
            GPASSERT(NULL!=c);
            GPASSERT(1 == c->size());
            GPASSERT(mContents.size() == mInputs.size());
            for (int i=0; i<mInputs.size(); ++i)
            {
                if (mInputs[i] == source)
                {
                    auto unit = (*c)[0];
                    if (NULL != mContents.get(i))
                    {
                        GPASSERT(mContents.contents[i].type == unit.type);
                        unit.type->vMerge(mContents.get(i), unit.content);
                    }
                    else
                    {
                        mContents.contents[i] = (*c)[0];
                    }
                    break;
                }
            }
            return false;
        }
    private:
        GPContents mContents;
    };
};
#endif /* defined(__GP__GPStreamADF__) */
