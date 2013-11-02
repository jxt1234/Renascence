#ifndef FUNCTION_GP_H
#define FUNCTION_GP_H

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include "user/package.h"

typedef GP_Output(*computeFunction)(std::vector<void*> inputs, std::vector<void*> constValue);

void GP_Output_clear(GP_Output& out);

#endif
