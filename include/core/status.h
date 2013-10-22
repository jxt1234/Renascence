#ifndef STATUS_H
#define STATUS_H

#include <vector>
#include <string>
#include <stdlib.h>
using namespace std;

typedef void*(*statusAllocMethod)();
typedef void(*statusVaryMethod)(void*);
typedef void(*statusCopyMethod)(void* src, void* dst);
typedef std::string(*statusPrintMethod)(void*);
typedef void*(*statusLoadMethod)(std::string);

/*Basic API*/
bool status_init();
int status_allocSet(const std::vector<int>& type);
int status_allocSet(int type);
int status_allocType(int size, string name, statusAllocMethod alloc=NULL, statusVaryMethod free=NULL, statusVaryMethod vary=NULL, 
        statusCopyMethod copy=NULL, statusPrintMethod print=NULL, statusLoadMethod load = NULL);
bool status_uploadSet(const std::vector<void*> src, int statusId);
int status_computesize(const std::vector<int>& type);
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
