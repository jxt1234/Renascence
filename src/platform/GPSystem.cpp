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
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <string>
#include <sstream>
#include "utils/GPDebug.h"
#include "platform/GPSystem.h"
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
        getcwd(current_absolute_path, PATH_MAX_SIZE);
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



std::vector<std::string> system_list_dir_files(const char* dir_name)
{
    GPASSERT(NULL!=dir_name);
    std::vector<std::string> result;
    auto dp = opendir(dir_name);
    GPASSERT(NULL!=dp);
    auto direntp = readdir(dp);
    while (NULL!=direntp)
    {
        /*Omit director*/
        if (direntp->d_type != DT_DIR)
        {
            result.push_back(std::string(direntp->d_name));
        }
        direntp = readdir(dp);
    }
    closedir(dp);
    return result;
}

