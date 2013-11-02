#include "utils/GP_Clock.h"
#include <time.h>
#include "utils/debug.h"
#include <stdio.h>

GP_Clock::GP_Clock(int func)
{
    mStart = clock();
    mId = func;
}

GP_Clock::~GP_Clock()
{
    int inter = clock()-mStart;
    printf("Function Id = %d, times = %dms+%dus\n", mId, inter/1000, inter%1000);
}
