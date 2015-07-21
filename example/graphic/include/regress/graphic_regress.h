#ifndef REGRESS_GRAPHIC_REGRESS_H
#define REGRESS_GRAPHIC_REGRESS_H

#include "graphic.h"
#include "transform_high.h"

typedef struct TrRegreeMode
{
    int* mode;
    int dim;
}TrRegreeMode;

/*Generate a TrFilterMatrix from src to target*/
/*Reverse function of TrFilterMatrixTransform*/
/*GP FUNCTION*/TrFilterMatrix* TrRegressMatrix(TrBmp* src, TrBmp* target, TrRegreeMode* mode);

/*Compute a amount of inputNumber float parameters from src to dst*/
/*Reverse function of TrMixPicture*/
void TrRegressMixFactor(TrBmp* dst, TrBmp** src, int inputNumber, float* parameters);



TrRegreeMode* TrRegreeModeAlloc(int dim);
void TrRegreeModeFree(TrRegreeMode* mode);
void TrRegreeModeCopy(TrRegreeMode* src, TrRegreeMode* dst);
TrRegreeMode* TrRegreeSingleModeAlloc(int dim);
int TrRegreeModeNumberCompute(TrRegreeMode* mode);

#endif
