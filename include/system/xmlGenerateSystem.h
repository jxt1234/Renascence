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
#ifndef XML_GENERATE_SYSTEM_H
#define XML_GENERATE_SYSTEM_H

#include "GenerateSystem.h"
#include "core/AbstractGP.h"
#include "user/IFunctionTable.h"
#include <ostream>
#include <istream>

class xmlGenerateSystem:public GenerateSystem
{
    public:
        xmlGenerateSystem();
        virtual ~xmlGenerateSystem();
        virtual void vAddContent(std::istream& is, IFunctionTable* table);
        void addXml(const char* xmlFile, IFunctionTable* table=NULL, bool print = false);
        void addXml(std::istream& is, IFunctionTable* table=NULL, bool print = false);
        void xmlPrint(std::ostream& out, AbstractGP* gp);
        virtual IGPAutoDefFunction* vCreateFunctionFromIS(std::istream& is);
        virtual IGPAutoDefFunction* vCreateFromADF(IGPAutoDefFunction* src);
    protected:
        virtual IGPAutoDefFunction* vCreateADFFromGP(AbstractGP* gp);
    private:
        std::vector<IFunctionTable*> mRemain;
};


class xmlGPADF:public IGPAutoDefFunction
{
    public:
        virtual GP_Output run(const GP_Input& inputs)
        {
            int cur = 0;
            mBase->input(inputs, cur);
            mBase->compute(mSys, mSys);
            return mBase->output();
        }
        virtual void save(std::ostream& os);
        virtual void load(std::istream& is);
        virtual void mutate();
        xmlGPADF(xmlGenerateSystem* sys, AbstractGP* base = NULL);
        xmlGPADF(const xmlGPADF& adf);
        void operator=(const xmlGPADF& adf);
        virtual ~xmlGPADF();
    private:
        xmlGenerateSystem* mSys;
        AbstractGP* mBase;
};


#endif
