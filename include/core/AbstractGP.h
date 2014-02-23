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
#include "math/AbstractPoint.h"
#include "function.h"
#include "IDataBase.h"
#include "IGPAutoDefFunction.h"
#include "utils/GP_Clock.h"
#include "utils/debug.h"
#include "status.h"

class AbstractGPPoint:public AbstractPoint
{
    public:
        AbstractGPPoint():mFunc(-1), mStatus(-1){}
        virtual ~AbstractGPPoint(){}
        inline int funcId(){return mFunc;}
        inline int statusId(){return mStatus;}
    protected:
        GP_Input mInputs;
        int mFunc;
        int mStatus;
};

class AbstractGP:public AbstractGPPoint, public IGPUnit
{
    public:
        //Basic Functions
        AbstractGP(int func, int status):mSave(NULL){mFunc = func, mStatus = status;}
        AbstractGP():mSave(NULL){}
        virtual ~AbstractGP();
        //Basic API
        virtual void compute(IRuntimeDataBase* map, statusBasic* sta);
        virtual void input(const GP_Input& input, int& cur);
        virtual GP_Output output();
        /*For Status free and copy*/
        std::vector<int> getStatus();
        /*Old API to cover GeneticProgram*/
        void reset();//Clear all mSave
        //If the functionIds is empty, it means to save all points
        void save(IRuntimeDataBase* map, statusBasic* sta, const std::vector<int>& functionIds);
        //Output a xml string, which can be write into file directly
        std::string xmlPrint(IPrintDataBase* data, statusBasic* sta);
        //The cur means from which element to use, it's assumed that the numbers is valid and the status has been alloc
        //FIXME: dangerous api
        static void loadUnitFunction(std::vector<int>& result, int functionId, int statusId, int number);
        /*Return functions that need input*/
        std::vector<int> setInputNumber(IRuntimeDataBase* map);
        class AbstractGPCopy:public AbstractPoint::IPointCopy
        {
            public:
                AbstractGPCopy(statusBasic* basic):mBasic(basic){}
                virtual ~AbstractGPCopy(){}
                virtual AbstractPoint* copy(AbstractPoint* src);
            private:
                statusBasic* mBasic;
        };
    protected:
        void replacePoint(const std::vector<int> &numbers, int& cur);
        void replacePoint(const std::vector<int> &numbers);
        GP_Output up_compute(IRuntimeDataBase* map, statusBasic* sta);
        void _reset();
        GP_Output* mSave;
        friend class GPProducer;
        //FIXME Reconstruct the xmlGPLoader
        friend class xmlGPLoader;
};


#endif
