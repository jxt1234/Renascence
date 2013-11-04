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
#ifndef STATUS_H
#define STATUS_H

#include <vector>
#include <string>
#include <stdlib.h>

typedef void*(*statusAllocMethod)();
typedef void(*statusVaryMethod)(void*);
typedef void(*statusCopyMethod)(void* src, void* dst);
typedef std::string(*statusPrintMethod)(void*);
typedef void*(*statusLoadMethod)(std::string);

/*Basic API*/
bool status_init();
int status_allocSet(const std::vector<int>& type);
int status_loadSet(const std::vector<int>& type, std::vector<std::string>& contents);
int status_allocType(int size, std::string name, statusAllocMethod alloc=NULL, statusVaryMethod free=NULL, statusVaryMethod vary=NULL, 
        statusCopyMethod copy=NULL, statusPrintMethod print=NULL, statusLoadMethod load = NULL);
bool status_freeSet(int statusId);
bool status_CopySet(int srcId, int dstId);
std::vector<void*> status_queryContent(int statusId);
const std::vector<int>& status_queryType(int statusId);
int status_CopyAllocSet(int srcId);
bool status_clear();

/*Optional API*/
bool status_varySet(int statusId);
std::string status_printSet(int statusId);
void status_init(const std::vector<int>& type, void* content);



#endif
