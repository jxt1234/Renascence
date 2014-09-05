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
#include "utils/GP_Clock.h"
#include "utils/debug.h"
#include "GPFunctionDataBase.h"
#include "IGPAutoDefFunction.h"

class GPTreeADFPoint:public AbstractPoint
{
    public:
        GPTreeADFPoint();
        virtual ~GPTreeADFPoint();
        void replacePoint(const std::vector<int> &numbers, const GPFunctionDataBase* base);
    private:
        /*Attributes*/
        const GPFunctionDataBase::function* mFunc;
        std::vector<GPStatusContent*> mStatus;
        /*Function needed*/
        void xmlPrint(std::ostream& res) const;
        //The cur means from which element to use, it's assumed that the numbers is valid and the status has been alloc
        GP_Output compute(const GP_Input& input, int& cur);
        //For recursive
        void getinput(std::vector<const IStatusType*>& tlist) const;

        class GPTreeADFCopy:public AbstractPoint::IPointCopy
        {
            public:
                GPTreeADFCopy(){}
                virtual ~GPTreeADFCopy(){}
                virtual AbstractPoint* copy(AbstractPoint* src);
        };
        void _replacePoint(const std::vector<int> &numbers, int& cur, const GPFunctionDataBase* base);
        /*FIXME Avoid friend class*/
        friend class GPTreeADF;
        friend class GPTreeProducer;
        friend class xmlCopy;
};

class GPTreeADF:public IGPAutoDefFunction
{
    public:
        //Basic Functions
        GPTreeADF();
        GPTreeADF(GPTreeADFPoint* root);
        virtual ~GPTreeADF();
        virtual IGPAutoDefFunction* copy() const;
        //Basic API
        GPTreeADFPoint* find(float rate);/*rate must be [0,1), return n*rate's element*/
        virtual GP_Output run(const GP_Input& inputs);
        /*Return all inputTypes in order*/
        virtual std::vector<const IStatusType*> vGetInputs() const;
        virtual std::vector<const IStatusType*> vGetOutputs() const;
        //Output a xml string, which can be write into file directly
        virtual void save(std::ostream& os) const;
        //FIXME: dangerous api
        static void loadUnitFunction(std::vector<int>& result, int functionId, int statusId, int number);
    protected:
        //FIXME Reconstruct the xmlGPLoader
        friend class xmlGPLoader;
    private:
        GPTreeADFPoint* mRoot;
};


#endif
