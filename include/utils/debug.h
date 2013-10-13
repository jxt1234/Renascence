#ifndef GENETIC_PROGRGAM_DEBUG_H
#define GENETIC_PROGRGAM_DEBUG_H

#include <stdlib.h>
#include <stdio.h>
#define FUNC_PRINT(x) printf(#x"=%d in %s, %d \n",x,  __func__, __LINE__);
#define FUNC_PRINT_ALL(x, type) printf(#x"="#type"%"#type" in %s, %d \n",x,  __func__, __LINE__);

#define CHECK_POINTER(x) {if(NULL==x){FUNC_PRINT_ALL(x,p);break;}}

#ifdef __cplusplus
extern "C"{
#endif
void dump_stack();
#ifdef __cplusplus
}
#endif

#endif
