/******************************************************************
 Copyright 2015, Jiang Xiao-tang
 
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
#ifndef USER_GPAPI_H
#define USER_GPAPI_H
class IGPAutoDefFunction;
#include <functional>
#include "GPContents.h"
#include "GPStream.h"
#include "IFunctionTable.h"
typedef std::vector<const IStatusType*> GPTYPES;
class AGPProducer;
enum
{
    GP_PRODUCER_TREE=0,
    GP_PRODUCER_GRAPHIC=1
};

/*Set basic path of lib*/
void GP_Set_Lib_Path(const char* basic_path);

void GP_Set_Stream_Path(const char* basic_path);

GPStream* GP_Stream_Create(const char* file);
void GP_Stream_Destroy(GPStream* s);

GPWStream* GP_WStream_Create(const char* file);
void GP_WStream_Destroy(GPWStream* s);

/*Create AGPProducer by function table, meta file and type
 * metaStream:
 * table: obtain the method to get function, can be NULL, then GP will create table by the path decribed by meta file
 * type: TREE:0, GRAPIC:1
 */
AGPProducer* GP_Producer_Create(GPStream* metaStream, IFunctionTable* table, int type);

/*Destroy the AGPProducer, before using this API, Make sure that all function created by p is destroyed*/
void GP_Producer_Destroy(AGPProducer* p);
/*Create a IGPAutoDefFunction by inputTypes and outputTypes, the types must be divided by space*/
/*For example: IGPAutoDefFunction* f = GP_Create_Function_ByType("SkBitmap SkPaint", "SkStream")*/
/*
 * p: the AGPProducer created by GP_Producer_Create
 * outputTypes: the name of all output types
 * inputTypes: all the input types obtained for the IGPAutoDefFunction, not orderred, if set "", GP will not care about the input
 * inputRepeat: whethether the type of inputTypes can be use repeatly
 */
/*Must remind that, user must get the real orderred inputTypes of the IGPAutoDefFunction after call this function*/
IGPAutoDefFunction* GP_Function_Create_ByType(const AGPProducer* p, const char* outputTypes, const char* inputTypes, bool inputRepeat);
/*Construct GP by formula like this: f(g(a, b), c)*/
IGPAutoDefFunction* GP_Function_Create_ByFormula(const AGPProducer* p, const char* formula);

/*Get All inputTypes in order*/
void GP_Function_Get_Inputs(const IGPAutoDefFunction* f, GPTYPES* output);
/*Get All outputTypes in order*/
void GP_Function_Get_Outputs(const IGPAutoDefFunction* f, GPTYPES* output);

/*The Inputs should be generate from stream by IStatusType inorder by the inputTypes return from GP_Function_Get_Inputs*/
/*The Outputs can be write to stream by IStatusType get from GP_Function_Get_Outputs*/
GPContents* GP_Function_Run(IGPAutoDefFunction* f, GPContents* input);
/*Free the memory of function*/
void GP_Function_Destroy(IGPAutoDefFunction* f);

/*Load f fromt input and Save f to output*/
/* The code below create a same function f2 from f1, in which the f1 come from the same producer
 GPStream is;
 GPWStream out;
 GP_Function_Save(f1, out);
 is(out.str());
 IGPAutoDefFunction* f2 = GP_Function_Create_ByStream(producer, is);
 */
IGPAutoDefFunction* GP_Function_Create_ByStream(const AGPProducer* p, GPStream* xmlFile);
void GP_Function_Save(IGPAutoDefFunction* f, GPWStream* output);

/*Optimize IGPAutoDefFunction by adjust parameters*/
/*REMIND: the f's parameters will be changed after this api*/
/*If fit_fun is not random, this api make sure that fit_fun(f) will at least not decrease*/
/*
 f: The IGPAutoDefFunction to be Optimize
 fit_fun: the function to compute the fitness of f, this api will find a max one
 type: 0 (use PSO, will compute maxTimes), 1(Use random golden divide Optimization, fast but not very accuracy)
 maxTimes: the maxTimes to run IGPAutoDefFunction in these api, will affect the algorithm of PSO and GDO, if set 0, GP will automatically determine how to terminate algorithm
 */
void GP_Function_Optimize(IGPAutoDefFunction* f, std::function< double(IGPAutoDefFunction*)> fit_fun, int type, const char* describe);
/*Evolution Method to find best function, it will take a long time*/
/*fit_func: the function to compute the fitness of IGPAutoDefFunction*/
/*TODO Let User control the speed of training*/
IGPAutoDefFunction* GP_Function_CreateBest_ByType(const AGPProducer* p, const char* outputTypes, const char* inputTypes, bool inputRepeat, std::function< double(IGPAutoDefFunction*)> fit_func, int maxTimes);
#endif
