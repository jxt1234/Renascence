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
#include "system/system_lib.h"
#include <dlfcn.h>
#include "utils/debug.h"

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
using namespace std;

/*TODO Add windows version*/

void* system_load_lib(const char* libName)
{
    ostringstream completeName;
    /*Read abosulute path form path.conf*/
    ifstream input("path.conf");
    string path;
    getline(input, path);
    completeName<<path << "/" << libName<<".so";
    return dlopen(completeName.str().c_str(), RTLD_NOW);
}

void* system_find_func(void* handle, const char* funcName)
{
    return dlsym(handle, funcName);
}


void system_unload_lib(void* handle)
{
    dlclose(handle);
}


