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
#ifndef GENETIC_PROGRGAM_DEBUG_H
#define GENETIC_PROGRGAM_DEBUG_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
/*Print method*/
#define GPPRINT(format, ...) printf(format,##__VA_ARGS__)
#define GPPRINT_FL(format,...) printf("FUNC: %s, LINE: %d: "format"\n", __func__, __LINE__,##__VA_ARGS__)

#define GPASSERT(x) assert(x)

/*Add with line and function*/
#define FUNC_PRINT(x) GPPRINT_FL(#x"=%d in %s, %d \n",x,  __func__, __LINE__);
#define FUNC_PRINT_ALL(x, type) GPPRINT(#x"= "#type" %"#type" in %s, %d \n",x,  __func__, __LINE__);

#define CHECK_POINTER(x) {if(NULL==x){FUNC_PRINT_ALL(x,p);break;}}

#ifdef __cplusplus
extern "C"{
#endif
void dump_stack();
#ifdef __cplusplus
}
#endif

#endif
