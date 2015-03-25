//
//  IGPAutoDefFunction.cpp
//  GP
//
//  Created by jiangxiaotang on 15/3/28.
//  Copyright (c) 2015年 jiangxiaotang. All rights reserved.
//

#include "core/IGPAutoDefFunction.h"


IGPAutoDefFunction* IGPAutoDefFunction::makeAdaptorFunction(IGPAutoDefFunction* base, const std::vector<size_t>& inputorder, const std::vector<const IStatusType*>& inputs)
{
    class adpatergp:public IGPAutoDefFunction
    {
    public:
        adpatergp(IGPAutoDefFunction* base, const std::vector<size_t>& order, const std::vector<const IStatusType*> inputs)
        {
            GPASSERT(NULL!=base);
            base->addRef();
            mBase = base;
            mInputOrder = order;
            mOutputTypes = base->vGetOutputs();
            mInputTypes = inputs;
        }
        ~adpatergp()
        {
            mBase->decRef();
        }
        virtual GPContents* run(GPContents* input)
        {
            GPContents newContents;
            for (int i=0; i<mInputOrder.size(); ++i)
            {
                auto id = mInputOrder[i];
                newContents.push(input->contents[id]);
            }
            return mBase->run(&newContents);
        }
        virtual void save(std::ostream& os) const
        {
            os << "<AdaptorGP>\n";
            os << "<Order>";
            for (auto id : mInputOrder)
            {
                os << id << " ";
            }
            os << "</Order>\n";
            os << "<BaseFunction>\n";
            mBase->save(os);
            os << "</BaseFunction>\n";
            os << "</AdaptorGP>\n";
        }
        virtual std::vector<const IStatusType*> vGetInputs() const
        {
            return mInputTypes;
        }
        /*Return all outputTypes in order*/
        virtual std::vector<const IStatusType*> vGetOutputs() const
        {
            return mOutputTypes;
        }
        
        virtual IGPAutoDefFunction* copy() const
        {
            IGPAutoDefFunction* r = new adpatergp(mBase, mInputOrder, mInputTypes);
            return r;
        }
        virtual int vMap(GPPtr<GPParameter> para)
        {
            return mBase->vMap(para);
        }
    private:
        IGPAutoDefFunction* mBase;
        std::vector<size_t> mInputOrder;
        std::vector<const IStatusType*> mInputTypes;
        std::vector<const IStatusType*> mOutputTypes;
    };
    return new adpatergp(base, inputorder, inputs);
}