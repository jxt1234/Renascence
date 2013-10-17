#ifndef SYSTEM_LIB_H
#define SYSTEM_LIB_H
void* system_load_lib(const char* libName);

void* system_find_func(void* handle, const char* funcName);

void system_unload_lib(void* handle);


#endif
