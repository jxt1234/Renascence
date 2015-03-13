void* GO_GP_Producer_Create(const char* metadataname, int type);
void GO_GP_Producer_Destroy(void* p);
void* GO_GP_Function_Create_ByType(void* p, const char* outputTypes, const char* inputTypes, int inputRepeat);
/*The files contents n file name, which is the same as types number*/
void* GO_GP_CreateContentsByTypes(void* p, const char* types, const char* files);
void* GO_GP_Function_Run(void* f, void* input);
void* GO_GP_Function_Create_ByFile(void* p, const char* file);
void GO_GP_Function_Save(void* f, const char* file);
