#include "system/system_lib.h"
#include <dlfcn.h>
#include "utils/debug.h"

#include <string>
#include <sstream>
using namespace std;

/*TODO Add windows version*/

void* system_load_lib(const char* libName)
{
    ostringstream completeName;
    completeName<<libName<<".so";
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


