/******************************************************************
   Copyright 2014, Jiang Xiao-tang

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
#include "producer/GPGraphicProducer.h"
#include "producer/GPGraphicADF.h"

IGPAutoDefFunction* GPGraphicProducer::vCreateFunction(const std::vector<const IStatusType*>& outputType, const std::vector<const IStatusType*>& inputType, bool inputRepeat, bool random)
{
    return NULL;
}
std::vector<IGPAutoDefFunction*> GPGraphicProducer::vCreateAllFunction(const std::vector<const IStatusType*>& outputType, const std::vector<const IStatusType*>& inputType, bool inputRepeat)
{
    std::vector<IGPAutoDefFunction*> res;
    return res;
}
IGPAutoDefFunction* GPGraphicProducer::vCreateFunctionFromName(const std::string& name)
{
    return NULL;
}
IGPAutoDefFunction* GPGraphicProducer::vCreateFunctionFromFormula(const std::string& formula)
{
    return NULL;
}
IGPAutoDefFunction* GPGraphicProducer::vCreateFunctionFromIS(std::istream& is)
{
    return new GPGraphicADF(is, mDataBase);
}
void GPGraphicProducer::vMutate(IGPAutoDefFunction* tree) const
{
}
GPGraphicProducer::GPGraphicProducer(const GPFunctionDataBase* base):mDataBase(base)
{
}
GPGraphicProducer::~GPGraphicProducer()
{
}
