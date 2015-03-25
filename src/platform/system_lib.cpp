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
/*TODO Add windows version*/
#include <dlfcn.h>
#include <unistd.h>

#include <string>
#include <sstream>
#include "utils/debug.h"
#include "system/system_lib.h"
using namespace std;

static std::string gPath("");
void system_set_path(const char* path)
{
    GPASSERT(NULL!=path);
    gPath = path;
}

#define PATH_MAX_SIZE 1024
void* system_load_lib(const char* libName)
{
    /*Get absolute path*/
    ostringstream completeName;
    if (gPath == "")
    {
        char current_absolute_path[PATH_MAX_SIZE];
        char* c = getcwd(current_absolute_path, PATH_MAX_SIZE);
        completeName<<current_absolute_path << "/" << libName<<".so";
    }
    else
    {
        completeName << gPath <<"/"<< libName << ".so";
    }
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


