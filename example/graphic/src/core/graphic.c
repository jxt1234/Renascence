#include "graphic.h"
#include <stdlib.h>
#include <string.h>

TrPixels TrPixelsBiOperate(TrPixels a, TrPixels b, TrBiOperator func)
{
    TrPixels c;
    c.r = func(a.r, b.r);
    c.g = func(a.g, b.g);
    c.b = func(a.b, b.b);
    return c;
}

int TrAbsPixels(TrPixels p)
{
    return (p.r+p.g+p.b);
}
int TrValidBmp(TrBmp* bmp)
{
    if (NULL==bmp) return 0;
    if (NULL==bmp->pixels||0>=bmp->width||0>=bmp->height) return 0;
    return 1;
}
void TrFreeBmp(TrBmp* bmp)
{
    if (NULL==bmp) return;
    if (NULL!=bmp->pixels)
    {
        free(bmp->pixels);
    }
    free(bmp);
}

TrBmp* TrAllocBmp(int width, int height)
{
    TrBmp* result = (TrBmp*)malloc(sizeof(TrBmp));
    result->width = width;
    result->height = height;
    result->pixels = (TrPixels*)malloc(sizeof(TrPixels)*width*height);
    return result;
}

TrBmp* TrCopyAllocBmp(TrBmp* src)
{
    TrBmp* dst;
    if (!TrValidBmp(src)) return NULL;
    dst = TrAllocBmp(src->width, src->height);
    memcpy((void*)(dst->pixels), (void*)(src->pixels), sizeof(TrPixels)*src->width*src->height);
    return dst;
}


#define ERROR_SQUARE(r1, r2, c) ((int)r1->c-(int)r2->c)*((int)r1->c-(int)r2->c)
double TrCompareBmp(TrBmp* src, TrBmp* dst)
{
    double result =0;
    if (!TrValidBmp(src) || !TrValidBmp(dst)) return -1;
    if (src->width!=dst->width || src->height!=dst->height) return -1;
    TrPixels* _src = src->pixels;
    TrPixels* _dst = dst->pixels;
    {
        int i;
        for (i=0; i<src->width*src->height; ++i)
        {
            result+= ERROR_SQUARE((_src+i), (_dst+i), r);
            result+= ERROR_SQUARE((_src+i), (_dst+i), g);
            result+= ERROR_SQUARE((_src+i), (_dst+i), b);
        }
    }
    return (result/3.0/src->width/src->height);
}
#undef ERROR_SQUARE
