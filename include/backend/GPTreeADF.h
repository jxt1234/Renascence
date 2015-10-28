/******************************************************************
 Copyright 2013, Jiang Xiao-tang
 
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
#ifndef CORE_ABSTRACTGP_H
#define CORE_ABSTRACTGP_H
#include <stdlib.h>
#include "math/GPAbstractPoint.h"
#include "user/GPContents.h"
#include "core/GPFunctionDataBase.h"
#include "core/IGPAutoDefFunction.h"
#include "core/GPStatusContent.h"
#include "utils/GP_Clock.h"
#include "utils/GPDebug.h"

class GPTreeADFPoint:public GPAbstractPoint
{
public:
    GPTreeADFPoint(const GPFunctionDataBase::function* func, size_t inputpos);
    void initStatus(const std::vector<std::istream*>& is);
    virtual ~GPTreeADFPoint();
    inline const GPFunctionDataBase::function* func() const {return mFunc;}
    inline const std::vector<GPStatusContent*>& status() {return mStatus;}
private:
    GPTreeADFPoint();
    /*Attributes*/
    const GPFunctionDataBase::function* mFunc;
    size_t mInputPos;
    std::vector<GPStatusContent*> mStatus;
    /*Function needed*/
    GPTreeNode* xmlPrint() const;
    GPContents* compute(GPContents* input);
    
    class GPTreeADFCopy:public GPAbstractPoint::IPointCopy
    {
    public:
        GPTreeADFCopy(){}
        virtual ~GPTreeADFCopy(){}
        virtual GPAbstractPoint* copy(GPAbstractPoint* src);
    };
    friend class GPTreeADF;
    friend class xmlCopy;
};

class GPTreeProducer;
class GPTreeADF:public IGPAutoDefFunction
{
public:
    //Basic Functions
    GPTreeADF(GPTreeADFPoint* root, const GPTreeProducer* p);
    virtual ~GPTreeADF();
    virtual IGPAutoDefFunction* vCopy() const;
    //Basic API
    inline GPTreeADFPoint* root() const {return mRoot;}
    GPTreeADFPoint* find(float rate);/*rate must be [0,1), return n*rate's element*/
    virtual GPContents* vRun(GPContents* inputs);
    virtual GPTreeNode* vSave() const;
    virtual int vMapStructure(GPParameter* para, bool& changed);
    virtual int vMap(GPParameter* para);
private:
    void _refreshInputsAndOutputs();
    GPTreeADFPoint* mRoot;
    const GPTreeProducer* mProducer;
};


#endif
