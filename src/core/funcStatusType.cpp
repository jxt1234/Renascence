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
using namespace std;

/*FIXME replace assert by exception*/
funcStatusType::funcStatusType(const std::string& name, IFunctionTable* table):IStatusType(name)
{
    assert(NULL!=table);
    string func;
    func = name + "_alloc";
    allocf = (statusAllocMethod)(table->vGetFunction(func));

    func = name + "_free";
    freef = (statusVaryMethod)(table->vGetFunction(func));

    func = name + "_vary";
    mutatef = (statusVaryMethod)(table->vGetFunction(func));

    func = name + "_copy";
    copyf = (statusCopyMethod)(table->vGetFunction(func));

    func = name + "_print";
    printvf = (statusPrintMethod)(table->vGetFunction(func));

    func = name + "_load";
    loadf = (statusLoadMethod)(table->vGetFunction(func));
    /*TODO Print which function is NULL*/    
}
void* funcStatusType::salloc() const
{
    assert(NULL!=allocf);
    return allocf();
}
void funcStatusType::sfree(void* contents) const
{
    assert(NULL!=freef);
    freef(contents);
}
void funcStatusType::mutate(void* contents) const
{
    assert(NULL!=mutatef);
    mutatef(contents);
}
void funcStatusType::copy(void* src, void* dst) const
{
    assert(NULL!=copyf);
    copyf(src, dst);
}
void funcStatusType::print(std::ostream& out, void* contents) const
{
    //assert(NULL!=printvf);
    if(NULL==printvf) return;
    string c = printvf(contents);
    out << c;
}
void* funcStatusType::load(std::istream& in) const
{
    //assert(NULL!=loadf);
    if(NULL==loadf) return NULL;
    /*The divide is assumed to be " "*/
    ostringstream os;
    string c;
    while(in >> c)
    {
        os << c << " ";
    }
    return loadf(os.str());
}
