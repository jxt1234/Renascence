/******************************************************************
   Copyright 2016, Jiang Xiao-tang

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
#ifndef MGPEXECUTOR_H
#define MGPEXECUTOR_H
#include "lowlevelAPI/IParallelMachine.h"
#include <queue>
#include "MGPThreadPool.h"
#include "backend/GPKeyIteratorFactory.h"
class MGPExecutor:public IParallelMachine::Executor
{
public:
    MGPExecutor(const GPParallelType* data, int threadNum, IParallelMachine::PARALLELTYPE type);
    virtual ~MGPExecutor();
    virtual bool vRun(GPPieces* output, GPPieces** inputs, int inputNumber) const;
    class ThreadData;
private:
    bool _mapRun(GPPieces* output, GPPieces** inputs, int inputNumber) const;
    bool _reduceRun(GPPieces* output, GPPieces** inputs, int inputNumber) const;
    
    GPPtr<IKeyFunction> mCondition;
    MGPThreadPool* mPool;
    ThreadData* mMainData;
    std::vector<ThreadData*> mUserData;
    IParallelMachine::PARALLELTYPE mType;
    GPParallelType::KEYS mOutputKey;
    GPParallelType::KEYS mVariableKey;
    GPPtr<GPKeyIteratorFactory> mFactory;
};
#endif
