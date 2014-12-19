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
#ifdef BUILD_FOR_ANDROID
#include <android/log.h>
#define GPPRINT(format, ...) __android_log_print(ANDROID_LOG_INFO, "GP", format,##__VA_ARGS__)
#define GPPRINT_FL(format,...) __android_log_print(ANDROID_LOG_INFO, "GP", format", FUNC: %s, LINE: %d \n",##__VA_ARGS__, __func__, __LINE__)
#else
#define GPPRINT(format, ...) printf(format,##__VA_ARGS__)
#define GPPRINT_FL(format,...) printf(format", FUNC: %s, LINE: %d \n", ##__VA_ARGS__,__func__, __LINE__)
#endif
/*Add with line and function*/
#define FUNC_PRINT(x) GPPRINT(#x"=%d in %s, %d \n",x,  __func__, __LINE__);
#define FUNC_PRINT_ALL(x, type) GPPRINT(#x"= "#type" %"#type" in %s, %d \n",x,  __func__, __LINE__);

#define CHECK_POINTER(x) {if(NULL==x){FUNC_PRINT_ALL(x,p);break;}}

#ifdef __cplusplus
extern "C"{
#endif
void dump_stack();
#ifdef __cplusplus
}
#endif
#ifndef BUILD_FOR_ANDROID
#define GPASSERT(x) assert(x)
#else
#define GPASSERT(x) \
    {bool result = (x);\
        if (!(result))\
        FUNC_PRINT((result));}
#endif

#endif
