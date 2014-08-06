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
#include <time.h>
#include "utils/debug.h"
#include "utils/GP_Clock.h"
#include "string.h"

GP_Clock::GP_Clock(int func, const char* name)
{
    mStart = clock();
    mId = func;
    int l = strlen(name);
    mName = new char[l+1];
    memcpy(mName, name, l);
    mName[l] = '\0';
}

GP_Clock::~GP_Clock()
{
    int inter = clock()-mStart;
    GPPRINT("%s __ %d, times = %dms+%dus\n", mName, mId, inter/1000, inter%1000);
    delete [] mName;
}
