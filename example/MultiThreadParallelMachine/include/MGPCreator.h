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
#ifndef MGPCREATOR_H
#define MGPCREATOR_H
#include "lowlevelAPI/IParallelMachine.h"
class MGPCreator:public IParallelMachine::Creator
{
public:
    virtual GPPieces* vPrepare(GPPieces** inputs, int inputNumber) const override;
    MGPCreator(const std::vector<std::pair<unsigned int, unsigned int>>& outputKey)
    {
        mOutputKey = outputKey;
    }
    virtual ~MGPCreator()
    {
    }
private:
    std::vector<std::pair<unsigned int, unsigned int>> mOutputKey;
};
#endif
