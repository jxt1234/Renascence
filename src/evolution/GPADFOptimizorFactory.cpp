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
#include "evolution/GPADFOptimizorFactory.h"

GPADFOptimizorFactory* GPADFOptimizorFactory::gFactory = NULL;
GPADFOptimizorFactory& GPADFOptimizorFactory::get()
{
    if (NULL == gFactory)
    {
        gFactory = new GPADFOptimizorFactory;
    }
    return *gFactory;
}
void GPADFOptimizorFactory::insert(const char* name, const char* decribe, IGPADFOptimizorCreator* t)
{
    GPASSERT(NULL!=name);
    GPASSERT(NULL!=decribe);
    GPASSERT(NULL!=t);
    if (name == NULL || decribe == NULL || t == NULL)
    {
        return;
    }
    std::string sname(name);
    std::string sdescribe(decribe);
    mDescribe.insert(std::make_pair(sname, sdescribe));
    mCreator.insert(std::make_pair(sname, t));
}

void GPADFOptimizorFactory::print(std::ostream& os)
{
    GPADFOptimizorFactory& f = GPADFOptimizorFactory::get();
    f._print(os);
}

GPPtr<IGPADFOptimizor> GPADFOptimizorFactory::create(const char* name, const GPProducer* producer, GPPtr<IGPFitComputer> fit)
{
    GPADFOptimizorFactory& f = GPADFOptimizorFactory::get();
    return f._create(name, producer, fit);
}

void GPADFOptimizorFactory::_print(std::ostream& os)
{
    EXPLAIN::const_iterator iter = mDescribe.begin();
    for (;iter!=mDescribe.end();iter++)
    {
        os << iter->first << std::endl;
        os << iter->second << std::endl;
        os << std::endl;
    }
}

GPPtr<IGPADFOptimizor> GPADFOptimizorFactory::_create(const char* name, const GPProducer* producer, GPPtr<IGPFitComputer> fit)
{
    GPPtr<IGPADFOptimizor> result;
    GPASSERT(NULL!=name);
    CREATERS::const_iterator iter = mCreator.find(name);
    if (iter!=mCreator.end())
    {
        result = iter->second->vCreate(producer, fit);
    }
    return result;
}

