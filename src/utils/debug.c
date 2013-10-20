#include "utils/debug.h"
void dump_stack()
{
#define SIZE 100
    int j, nptrs;
    void* buffer[SIZE];
    char** strings;
    nptrs = backtrace(buffer, SIZE);
    printf("backtrace() returned %d, addresses \n", nptrs);
    strings = (char**)backtrace_symbols(buffer, nptrs);
    if (strings==NULL)
    {
        return;
    }
    for (j=0; j<nptrs; ++j)
    {
        printf("%s\n", strings[j]);
    }
    free(strings);
}
