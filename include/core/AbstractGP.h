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
#include "algorithm/AbstractPoint.h"
#include "function.h"
#include "IDataBase.h"
#include "IGPRunner.h"
#include "utils/GP_Clock.h"
#include "utils/debug.h"
#include "status.h"
class AbstractGPPoint:public AbstractPoint
{
    public:
        AbstractGPPoint():mFunc(-1), mStatus(-1){}
        virtual ~AbstractGPPoint(){}
    protected:
        int mFunc;
        int mStatus;
};

class AbstractGP:public AbstractGPPoint, IGPRunner
{
    public:
        //Basic Functions
        AbstractGP():mSave(NULL){}
        AbstractGP(const AbstractGP& gp);
        void operator=(const AbstractGP& gp);
        virtual ~AbstractGP();
        //Basic API
        virtual void compute(IFunctionDataBase* map);
        virtual void input(GP_Input& input);
        virtual GP_Output output();
        /*Old API to cover GeneticProgram*/
        void reset();//Clear all mSave
        //If the functionIds is empty, it means to save all points
        void save(IFunctionDataBase* map, const std::vector<int>& functionIds);
        //The cur means from which element to use, it's assumed that the numbers is valid and the status has been alloc
        //FIXME: dangerous api
        void replacePoint(const std::vector<int> &numbers, int& cur);
        //Output a xml string, which can be write into file directly
        std::string xmlPrint(IDataBase* data);
    protected:
        GP_Output up_compute(IFunctionDataBase* map);
        void _reset();
        class AbstractGPCopy:public AbstractPoint::IPointCopy
        {
            public:
                virtual AbstractPoint* copy(AbstractPoint* src);
        };
        GP_Output* mSave;
};


#endif
