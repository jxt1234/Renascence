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
#include "utils/GPDebug.h"
#include "utils/GPClock.h"
#include <string.h>
#include <stdlib.h>

GPClock::GPClock(int func, const char* name)
{
    mStart = clock();
    mId = func;
    mName = ::strdup(name);
}

GPClock::~GPClock()
{
    auto inter = clock()-mStart;
    GPPRINT("%s __ %d, times = %ldms+%ldus\n", mName, mId, inter/1000, inter%1000);
    ::free(mName);
}
