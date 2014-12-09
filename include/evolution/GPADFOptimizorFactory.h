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
#ifndef EVOLUTION_GPADFOPTIMIZORFACTORY_H
#define EVOLUTION_GPADFOPTIMIZORFACTORY_H
#include <ostream>
#include <map>
#include <string>
#include "IGPADFOptimizor.h"
#include "core/GPProducer.h"
class IGPFitComputer:public RefCount
{
    public:
        virtual PFLOAT compute(IGPAutoDefFunction* f) const = 0;
        IGPFitComputer(){}
        virtual ~IGPFitComputer(){}
};
class IGPADFOptimizorCreator:public RefCount
{
    public:
        virtual GPPtr<IGPADFOptimizor> vCreate(const GPProducer* producer, GPPtr<IGPFitComputer> fit) const = 0;
        IGPADFOptimizorCreator(){}
        virtual ~IGPADFOptimizorCreator(){}
};
class GPADFOptimizorFactory
{
    public:
        static GPPtr<IGPADFOptimizor> create(const char* name, const GPProducer* producer, GPPtr<IGPFitComputer> fit);
        static void print(std::ostream& os);
        static GPADFOptimizorFactory& get();
        void insert(const char* name, const char* decribe, IGPADFOptimizorCreator* t);
    private:
        void _print(std::ostream& os);
        GPPtr<IGPADFOptimizor> _create(const char* name, const GPProducer* producer, GPPtr<IGPFitComputer> fit);
        static GPADFOptimizorFactory* gFactory;
        typedef std::map<std::string, IGPADFOptimizorCreator*> CREATERS;
        typedef std::map<std::string, std::string> EXPLAIN;
        CREATERS mCreator;
        EXPLAIN mDescribe;
};
template<class T>
class GPADFOptimizorRegistor
{
    public:
        GPADFOptimizorRegistor(const char* name, const char* decribe)
        {
            T* creator = new T;
            GPADFOptimizorFactory& f = GPADFOptimizorFactory::get();
            f.insert(name, decribe, creator);
        }
        ~GPADFOptimizorRegistor(){}
};
#endif
