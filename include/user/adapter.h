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
#ifndef USER_ADAPTER_H
#define USER_ADAPTER_H
void* GO_GP_Producer_Create(const char* metadataname, int type);
void GO_GP_Producer_Destroy(void* p);
void* GO_GP_Function_Create_ByType(void* p, const char* outputTypes, const char* inputTypes, int inputRepeat);
/*The files contents n file name, which is the same as types number*/
void* GO_GP_CreateContentsByTypes(void* p, const char* types, const char* files);
void* GO_GP_Function_Run(void* f, void* input);
void* GO_GP_Function_Create_ByFile(void* p, const char* file);
void GO_GP_Function_Save(void* f, const char* file);
#endif
