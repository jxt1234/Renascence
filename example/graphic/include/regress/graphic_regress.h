#ifndef REGRESS_GRAPHIC_REGRESS_H
#define REGRESS_GRAPHIC_REGRESS_H

#include "graphic.h"
#include "transform_high.h"
#include "regress/f2c.h"
#include "regress/clapack.h"

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


/*Currently privately, but may be used as public in future*/
/*Expand the src to a M*N float matrix, with the TrRegreeMode mode*/
void TrRegressExpand(TrBmp* src, TrRegreeMode* mode, float* X, integer* M, integer* N, int hasConst);


TrRegreeMode* TrRegreeModeAlloc(int dim);
void TrRegreeModeFree(TrRegreeMode* mode);
void TrRegreeModeCopy(TrRegreeMode* src, TrRegreeMode* dst);
TrRegreeMode* TrRegreeSingleModeAlloc(int dim);
int TrRegreeModeNumberCompute(TrRegreeMode* mode);

#endif
