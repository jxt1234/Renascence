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
#include "utils/debug.h"
#include "core/funcStatusType.h"
#include <assert.h>
#include <sstream>
#include <iostream>
#include "utils/GPRandom.h"
using namespace std;

/*FIXME replace assert by exception*/
funcStatusType::funcStatusType(const std::string& name, IFunctionTable* table):IStatusType(name)
{
    assert(NULL!=table);
    string func;
    func = name + "_alloc";
    allocf = (statusAllocMethod)(table->vGetFunction(func));

    func = name + "_free";
    freef = (statusFreeMethod)(table->vGetFunction(func));

    func = name + "_vary";
    mutatef = (statusVaryMethod)(table->vGetFunction(func));

    func = name + "_map";
    mapf = (statusMapMethod)(table->vGetFunction(func));

    func = name + "_copy";
    copyf = (statusCopyMethod)(table->vGetFunction(func));

    func = name + "_print";
    printvf = (statusPrintMethod)(table->vGetFunction(func));

    func = name + "_load";
    loadf = (statusLoadMethod)(table->vGetFunction(func));
    /*TODO Print which function is NULL*/
}
void* funcStatusType::Alloc() const
{
    assert(NULL!=allocf);
    return allocf();
}
void funcStatusType::Free(void* contents) const
{
    assert(NULL!=freef);
    freef(contents);
}
void funcStatusType::mutate(void* contents) const
{
    assert(NULL!=mutatef || NULL!=mapf);
    if (NULL != mutatef)
    {
        mutatef(contents);
    }
    else if(NULL!=mapf)
    {
        double f = GPRandom::rate();
        mapf(contents, f);
    }
}

void funcStatusType::mapValue(void* contents, double value) const
{
    if (NULL!=mapf)
    {
        mapf(contents, value);
    }
}
void funcStatusType::copy(void* src, void* dst) const
{
    assert(NULL!=copyf);
    copyf(src, dst);
}
void funcStatusType::print(std::ostream& out, void* contents) const
{
    if(NULL==printvf) return;
    printvf(out, contents);
}
void* funcStatusType::load(std::istream& in) const
{
    if(NULL==loadf) return NULL;
    return loadf(in);
}
