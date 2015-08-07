//
//  IGPAutoDefFunction.cpp
//  GP
//
//  Created by jiangxiaotang on 15/3/28.
//  Copyright (c) 2015年 jiangxiaotang. All rights reserved.
//

#include "core/IGPAutoDefFunction.h"
#include <sstream>


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
            mOutputTypes = base->getOutputTypes();
            mInputTypes = inputs;
        }
        ~adpatergp()
        {
            mBase->decRef();
        }
        virtual GPContents* vRun(GPContents* input)
        {
            GPContents newContents;
            for (int i=0; i<mInputOrder.size(); ++i)
            {
                auto id = mInputOrder[i];
                newContents.push(input->contents[id]);
            }
            return mBase->vRun(&newContents);
        }
        virtual GPTreeNode* vSave() const
        {
            GPTreeNode* root = new GPTreeNode("AdaptorGP", "");
            std::ostringstream orderid;
            for (auto id : mInputOrder)
            {
                orderid << id << " ";
            }
            root->addChild(new GPTreeNode("Order", orderid.str()));
            root->addChild(mBase->vSave());
            return root;
        }
        
        virtual IGPAutoDefFunction* vCopy() const
        {
            IGPAutoDefFunction* r = new adpatergp(mBase, mInputOrder, mInputTypes);
            return r;
        }
        virtual int vMap(GPPtr<GPParameter> para)
        {
            return mBase->vMap(para);
        }
        virtual void vMutate()
        {
            mBase->vMutate();
        }
    private:
        IGPAutoDefFunction* mBase;
        std::vector<size_t> mInputOrder;
    };
    return new adpatergp(base, inputorder, inputs);
}