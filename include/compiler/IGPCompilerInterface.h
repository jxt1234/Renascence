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
#ifndef COMPILER_IGPCOMPILERINTERFACE_H
#define COMPILER_IGPCOMPILERINTERFACE_H

#include "proto/GPDAG.pb-c.h"
#include "lowlevelAPI/IGPFunction.h"
class IGPFrontEnd : public GPRefCount
{
public:
    virtual GP__PointGroup* vCreate(const char* content, char** errorInfo) const = 0;
    virtual ~ IGPFrontEnd(){}

protected:
    IGPFrontEnd(){}
};

//Expand ADF or mutate expanded ADF
class IGPAdaptor : public GPRefCount
{
public:
    //If the origin has no ADF, Do nothing and return false
    virtual bool vExpand(GP__ADF* origin) const = 0;
    virtual int vMapStructure(GP__ADF* target, const double* paramters, int n_paramters) const = 0;
    virtual ~ IGPAdaptor(){}
protected:
    IGPAdaptor(){}
};

//Turn PointGroup to DAG. Do Optimization Here. The PointGroup shouldn't has ADF
class IGPMidEnd : public GPRefCount
{
public:
    virtual GP__DAG* vCreate(const GP__PointGroup* front) const = 0;
    virtual ~ IGPMidEnd(){}
protected:
    IGPMidEnd(){}
};

//Turn DAG to Function
class IGPBackEnd : public GPRefCount
{
public:
    virtual IGPFunction* vCreate(const GP__DAG* dag) const = 0;
    virtual ~ IGPBackEnd(){}
protected:
    IGPBackEnd(){}
};

#endif
