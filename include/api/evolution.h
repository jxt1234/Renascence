#ifndef API_EVOLUTION_H
#define API_EVOLUTION_H

#include "core/function.h"

extern "C"{
/*Input: number, times, functionTable, inputTable; Output: outputXml*/
void GP_evolution(int number, int times, const char* functionTable, const char* inputTable, const char* outputXml);
void GP_RunXml(const char* functionXml, const char* input, const char* output);
}


#endif
