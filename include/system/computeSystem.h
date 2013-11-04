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
#ifndef GP_COMPUTE_SYSTEM_H
#define GP_COMPUTE_SYSTEM_H
#include "core/function.h"
#include "core/status.h"
#include "xml/xmlFunctionLoader.h"

class computeSystem
{
    public:
        struct function
        {
            computeFunction basic;
            std::vector<int> outputType;
            std::vector<int> statusType;
            std::vector<std::vector<int> > fixTable;
        };
        computeFunction getFunction(int id);
        void loadFuncXml(xmlFunctionLoader& loader, void* &handle);
        void clear();
        //Basic Api
        int allocateStatus(int id);
        inline const std::vector<int>& getInputFunctions(){return mInputId;}
        const std::vector<std::vector<int> >& getAvailableFunctionInputs(int functionId);
        const std::vector<int>& getOutputFunctions(){return mOutputId;}
        void print();
        computeSystem();
        virtual ~computeSystem();
    protected:
        std::vector<int> loadStatus(const std::vector<xmlFunctionLoader::status>& sta, void* handle);
        void constructBasicTable();
    private:
        void _loadUnit(xmlFunctionLoader func);
        std::vector<function*> mFunctionTable;
        std::vector<int> mOutputId;//可做为根结点的函数Id
        std::vector<int> mInputId;//作为叶结点的函数Id
};

#endif
