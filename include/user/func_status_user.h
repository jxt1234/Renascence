#ifndef FUNCTION_STATUS_USER_H
#define FUNCTION_STATUS_USER_H
#include "core/function.h"
#include "core/status.h"
extern computeFunction* gAllFunctions;
extern int gAllFunctionsNumber;
void initFunctions();
void destroyFunction();
vector<int> initStatusType();
void destroyStatus();
#endif
