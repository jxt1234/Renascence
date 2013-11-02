#ifndef API_EVOLUTION_H
#define API_EVOLUTION_H

#include "core/function.h"

extern "C"{
/*Input: number, times, functionTable, inputTable; Output: outputXml*/
void GP_evolution(int number, int times, const char* functionTable, const char* inputTable, const char* outputXml);
/*Input: functionTree's xml, input resource, output function to treat GP_Output*/
/*The GP_Output for the output function will be free in this function*/
void GP_RunXml(const char* functionXml, const char* input, void(*output)(GP_Output&));
}


#endif
