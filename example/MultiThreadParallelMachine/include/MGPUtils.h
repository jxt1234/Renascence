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
#ifndef MGPUTILS_H
#define MGPUTILS_H
#include <assert.h>
#define MGPASSERT(x) assert(x)
#ifdef BUILD_FOR_ANDROID
#include <android/log.h>
#define MGPPRINT(format, ...) __android_log_print(ANDROID_LOG_INFO, "GP", format,##__VA_ARGS__)
#else
#define MGPPRINT(format, ...) printf(format,##__VA_ARGS__)
#endif
#define FUNC_PRINT(x) MGPPRINT(#x"=%d in %s, %d \n",x,  __func__, __LINE__);
#define FUNC_PRINT_ALL(x, type) MGPPRINT(#x"= "#type" %"#type" in %s, %d \n",x,  __func__, __LINE__);
#endif
