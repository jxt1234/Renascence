#ifndef TR_BITMAP_GRAPHIC_H
#define TR_BITMAP_GRAPHIC_H
#include <stdio.h>
#include <stdlib.h>
typedef unsigned char uchar;
typedef uchar(*TrBiOperator)(uchar, uchar);
typedef struct TrPixels
{
    uchar r;
    uchar g;
    uchar b;
}TrPixels;

typedef struct TrBmp
{
    int width;
    int height;
    TrPixels* pixels;
}TrBmp;

TrPixels TrPixelsBiOperate(TrPixels a, TrPixels b, TrBiOperator func);
int TrAbsPixels(TrPixels p);
int TrValidBmp(TrBmp* bmp);
TrBmp* TrAllocBmp(int width, int height);
TrBmp* TrCopyAllocBmp(TrBmp* src);
void TrFreeBmp(TrBmp* bmp);
double TrCompareBmp(TrBmp* src, TrBmp* dst);
#endif
