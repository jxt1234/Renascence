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

#include "producer/GPStreamADFProducer.h"
#include "GPStreamADF.h"

GPStreamADFProducer::GPStreamADFProducer(const GPFunctionDataBase* sys):GPProducer("GPStreamADFProducer")
{
    mDataBase = sys;
}
GPStreamADFProducer::~GPStreamADFProducer()
{
}


IGPAutoDefFunction* GPStreamADFProducer::vCreateFunction(const std::vector<const IStatusType*>& outputType, const std::vector<const IStatusType*>& inputType, bool inputRepeat, bool random) const
{
    return NULL;
}
std::vector<IGPAutoDefFunction*> GPStreamADFProducer::vCreateAllFunction(const std::vector<const IStatusType*>& outputType, const std::vector<const IStatusType*>& inputType, bool inputRepeat) const
{
    std::vector<IGPAutoDefFunction*> res;
    return res;
}
IGPAutoDefFunction* GPStreamADFProducer::vCreateFunctionFromFormula(const std::string& formula) const
{
    return NULL;
}
IGPAutoDefFunction* GPStreamADFProducer::vCreateFunctionFromNode(const GPTreeNode* p) const
{
    GPASSERT(NULL!=p);
    if (p->name() != "GPStreamADF")
    {
        return NULL;
    }
    return new GPStreamADF(p, mDataBase);
}


