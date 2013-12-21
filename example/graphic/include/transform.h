#ifndef TR_TRANSFORM_H
#define TR_TRANSFORM_H

#include "graphic.h"
#define Tr_FLOAT_LIMIT_TO_CHAR(x) ((x)>255?255:(x)<0?0:(unsigned char)(x))
#define Tr_DOUBLE_TO_CHAR(x) x*256
#define Tr_MULTI(x, a) ((x)*(a))>>8;
#define Tr_BLEND(x1, x2, a) Tr_MULTI(x1, 255-a) + Tr_MULTI(x2, a)
#define Tr_LIMIT(x, w) (x)=(x)<0?0:(x);(x)=(x)%(w);

void TrMultyPixels(TrBmp* src, double factor);

TrBmp* TrMixturePixels(TrBmp* src1, TrBmp* src2, double alpha);

TrBmp* TrLoadPixels(const char* file);

void TrWritePixels(TrBmp* src, const char* file);
TrBmp* TrScaleBmp(TrBmp* src, int targetWidth, int targetHeight);

typedef void(*PixelsSwapFunc)(TrPixels* p);
TrBmp* TrColorSwap(TrBmp* src, PixelsSwapFunc func);
TrBmp* TrColorReverse(TrBmp* src);
TrBmp* TrMixPicture(TrBmp** pictures, float* factor, int n, int width, int height);

#endif
