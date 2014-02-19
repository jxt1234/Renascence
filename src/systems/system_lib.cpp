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

#include "system/system_lib.h"
#include "utils/debug.h"

void* system_multi_lib::vGetFunction(const std::string& name)
{
    computeFunction res = NULL;
    for (int i=0; i<mHandles.size(); ++i)
    {
        res = system_find_func(mHandles[i], name.c_str());
        if (NULL!=res)
        {
            break;
        }
    }
    return res;
}

void system_multi_lib::addLib(const char* name)
{
    void* handle = system_load_lib(name);
    if (NULL==handle)
    {
        FUNC_PRINT_ALL(handle, p);
        return;
    }
    mHandles.push_back(handle);
}

system_multi_lib::~system_multi_lib()
{
    for (int i=0; i<mHandles.size(); ++i)
    {
        system_unload_lib(mHandles[i]);
    }
}
