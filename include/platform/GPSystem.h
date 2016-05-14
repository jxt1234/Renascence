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
#ifndef PLATFORM_GPSYSTEM_H
#define PLATFORM_GPSYSTEM_H
#include <string>
#include <vector>
void system_set_path(const char* path);
void* system_load_lib(const char* libName);
void* system_find_func(void* handle, const char* funcName);
void system_unload_lib(void* handle);


std::vector<std::string> system_list_dir_files(const char* dir_name);
bool system_make_dir(const char* dir_name);
bool system_rm_dir(const char* dir_name);
#endif
