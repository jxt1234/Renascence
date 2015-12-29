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
#ifdef __cplusplus
extern "C"{
#endif
    typedef struct IGPAutoDefFunction IGPAutoDefFunction;
    typedef struct AGPProducer AGPProducer;
    typedef struct GPStream GPStream;
    typedef struct GPContents GPContents;
    typedef struct GPWStream GPWStream;
    typedef struct IFunctionTable IFunctionTable;
    enum
    {
        GP_PRODUCER_TREE=0,
        GP_PRODUCER_GRAPHIC=1,
        GP_PRODUCER_STREAM=2
    };
    /*Set basic path of lib
     * The lib will be loaded  from basic_path/
     * For Example, set basic_path as /a/b/c/
     * Then will load liba,so from /a/b/c/liba.so
     * */
    void GP_Set_Lib_Path(const char* basic_path);
    
    /*Set Basic path for File Stream, Affect the API GP_Stream_Create and GP_WStream_Create
     * Same as GP_Set_Lib_Path
     * */
    void GP_Set_Stream_Path(const char* basic_path);
    
    /*Create Read Stream for file, using rb mode. The Real file is basic_path + filename*/
    GPStream* GP_Stream_Create(const char* filename);
    /*Destroy the stream created by GP_Stream_Create, must not be used to destroy other Stream*/
    void GP_Stream_Destroy(GPStream* s);
    
    /*Create Write Stream for file, using wb mode. The Real file is basic_path + filename*/
    GPWStream* GP_WStream_Create(const char* filename);
    /*Destroy the stream created by GP_WStream_Create, must not be used to destroy other Stream*/
    void GP_WStream_Destroy(GPWStream* s);
    
    /* Load Contents by IStatusType's vLoad method, Cona GPContents
     * producer: the FunctionProducer
     * inputs: the list of Stream, must assume that all input are not NULL
     * typeNames: the list of types
     * n: select number, must be equal to types
     * For Example:
     *      GPContents* c = GP_Contents_Load(producer, inputs, "TrBmp TrBmp", 2);
     */
    GPContents* GP_Contents_Load(AGPProducer* producer, GPStream** inputs, const char* typeNames, int n);
    
    /* Write Contents to appointed stream
     * content: the contents to be write, must not be null
     * outputs: the array of output stream, must not be null, all stream in this list should not be null, too
     * n: the number of stream, must be the same as content's size
     * Example:
     *      outputStream = GP_WStream_Create("test.txt");
     *      GP_Contents_Save(contents, &outputStream, 1);
     *
     */
    void GP_Contents_Save(GPContents* content, GPWStream** outputs, int n);
    
    /*Destroy the contents in GPContents and itself
     * content: The memory to be freed
     */
    void GP_Contents_Destroy(GPContents* content);
    
    
    /*Create AGPProducer by function table, meta file and type
     * metaStream:
     * table: obtain the method to get function, can be NULL, then GP will create table by the path decribed by meta file
     * n: the number of stream and tables
     * type: ALL:-1, TREE:0, GRAPIC:1
     */
    AGPProducer* GP_Producer_Create(GPStream** metaStream, IFunctionTable** table, int n, int type);
    
    /*Destroy the AGPProducer, before using this API, Make sure that all function created by p is destroyed*/
    void GP_Producer_Destroy(AGPProducer* p);
    
    
    typedef struct GPOptimizorInfo
    {
        /*The Max Tree Depth For all ADF Function
         * For Example:
         * If The nMaxADFDepth is set as 1, then ADF(x0, x1) can only be f(x0, x1), can't be f(g(x0), x1)
         */
        int nMaxADFDepth;
        
        /*Determine which method for optimization. Make different sense for structure and parameter*/
        int nOptimizeType;
        
        /*The Function to Determine whether the Auto-Generate-Function is good, the larger the better*/
        double(*pFitComputeFunction)(IGPAutoDefFunction* adf, void* pMeta);
        void* pMeta;

        /*The max time for pFitComputeFunction to call in optimization*/
        int nMaxRunTimes;
    }GPOptimizorInfo;
    
    /*Create a IGPAutoDefFunction by inputTypes and outputTypes, the types must be divided by space*/
    /*For example: IGPAutoDefFunction* f = GP_Create_Function_ByType(p, "SkBitmap SkPaint", "SkStream", false)*/
    /*
     * p: the AGPProducer created by GP_Producer_Create
     * outputTypes: the name of all output types
     * inputTypes: all the input types obtained for the IGPAutoDefFunction, not orderred, if set "", GP will not care about the input
     */
    IGPAutoDefFunction* GP_Function_Create_ByType(const AGPProducer* p, const char* outputTypes, const char* inputTypes, GPOptimizorInfo* pInfo);
    
    /*Create GP by formula like this: f(g(a, b), c)
     * p: the AGPProducer created by GP_Producer_Create
     * formula: the string of formula
     * inputType: the string of all types
     * Example: auto adf = GP_Function_Create_ByFormula(p, "f(x0, g(x1))", "Matrix Matrix", NULL);
     */
    IGPAutoDefFunction* GP_Function_Create_ByFormula(const AGPProducer* p, const char* formula, const char* inputType, GPOptimizorInfo* pInfo);
    
    /*The Inputs should be generate from stream by IStatusType inorder by the inputTypes return from GP_Function_Get_Inputs*/
    /*The Outputs can be write to stream by IStatusType get from GP_Function_Get_Outputs*/
    GPContents* GP_Function_Run(IGPAutoDefFunction* f, GPContents* input);
    /*Free the memory of function*/
    void GP_Function_Destroy(IGPAutoDefFunction* f);
    
    /*Load f from input stream, which mainly come from a xmlfile*/
    IGPAutoDefFunction* GP_Function_Create_ByStream(const AGPProducer* p, GPStream* xmlFile);
    
    /*Save f to output, use xml format*/
    void GP_Function_Save(IGPAutoDefFunction* f, GPWStream* output);
    
    /*Optimize IGPAutoDefFunction by adjust its parameters*/
    /*REMIND: the f's parameters will be changed after this api*/
    /*If fit_fun is not random, this api make sure that fit_fun(f) will at least not decrease*/
    /*
     f: The IGPAutoDefFunction to be Optimize
     pInfo: obtain function to determine f's fit, nMaxRunTimes is the maxTimes to run IGPAutoDefFunction in these api, will affect the algorithm of PSO. The nMaxADFDepth make no sense
     pInfo.nOptimizeType: 0 (use PSO, will compute maxTimes), 1(Use random golden divide Optimization, fast but not very accuracy)
     
     example: 
     {
        struct GPOptimizorInfo info;
        info.nMaxRunTimes = 1000;
        info.pFitComputeFunction = f;
        info.pMeta = meta;
        GP_Function_Optimize(f, &info);
     }
     */
    void GP_Function_Optimize(IGPAutoDefFunction* f, GPOptimizorInfo* pInfo);
#ifdef __cplusplus
}
#endif
#endif
