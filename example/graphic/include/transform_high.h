#ifndef TR_TRANSFORM_HIGH_LEVEL_ALGORITHM
#define TR_TRANSFORM_HIGH_LEVEL_ALGORITHM
#include "transform.h"
//Create a new TrBmp as normal blur of src
TrBmp* TrNormalBlur(TrBmp* src);
TrBmp* TrSimpleSharp(TrBmp* src, int amount, int threshold);

/*滤波算法相关*/
/*Filter algorithm*/
typedef struct TrFilterData
{
    TrPixels** data;
    int number;
}TrFilterData;
/*x is for width, while y is for height*/
/*The memory of TrFilterData should be allocated by TrLoadFunc, and freed by TrReduceFunc*/
typedef TrFilterData*(*TrLoadFunc)(TrBmp* src, int x, int y);
typedef TrPixels(*TrReduceFunc)(TrFilterData* data);

TrFilterData* TrFilterDataAlloc(int number);
void TrFilterDataFree(TrFilterData* data);

/*Will allocate a new TrBmp that has the same size with src*/
TrBmp* TrFilterTransform(TrBmp* src, TrLoadFunc load, TrReduceFunc reduce);

/*以下算法均以滤波算法为基础*/
/*The algorithms below are based on Filter algorithm*/

/*浮雕*/
TrBmp* TrFilterRelief(TrBmp* src);
/*霓虹*/
TrBmp* TrFilterRainbow(TrBmp* src);



/*Matrix filter algorithm*/
/*矩阵滤波算法，根据取值矩阵，得出滤波值，最为常用的滤波算法系列*/
typedef struct TrFilterMatrix
{
    float* data;
    int dim;
    int offset;
}TrFilterMatrix;

TrFilterMatrix* TrFilterMatrixAlloc(int dim);
TrFilterMatrix* TrFilterMatrixCopyAlloc(float* src, int dim);
char* TrFilterMatrixPrint(TrFilterMatrix* m);
void TrFilterMatrixFree(TrFilterMatrix* m);


TrPixels TrFilterMatrixCompute(TrBmp* src, int x, int y, TrFilterMatrix* modle);

TrBmp* TrFilterMatrixTransform(TrBmp* src, TrFilterMatrix* matrix);

TrBmp* TrSaturation(TrBmp* src, float satValue);
#endif
