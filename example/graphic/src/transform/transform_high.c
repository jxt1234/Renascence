#include "transform_high.h"
//Nine average
static TrPixels _TrNormalBlurUnit(int x, int y, TrBmp* src)
{
    int r=0,g=0,b=0;
    int i, j;
#define addValue(r, x,y) r = r+(src->pixels+y*src->width+x)->r;
    for (i=-1; i<=1; ++i)
    {
        for (j=-1; j<=1;++j)
        {
            addValue(r,(x+i),(y+j) );
            addValue(g,(x+i),(y+j) );
            addValue(b,(x+i),(y+j) );
        }
    }
    TrPixels p;
    p.r = r/9;
    p.g = g/9;
    p.b = b/9;
    return p;
}

TrBmp* TrNormalBlur(TrBmp* src)
{
    TrBmp* dst;
    int i, j;
    if (!TrValidBmp(src)) return NULL;
    dst = TrCopyAllocBmp(src);
    for (i=1; i<dst->width-1; ++i)
    {
        for (j=1; j<dst->height-1; ++j)
        {
            *(dst->pixels+j*dst->width+i)=_TrNormalBlurUnit(i, j, src);
        }
    }
    return dst;
}

static uchar _minus(uchar a, uchar b)
{
    if (a<b) return 0;
    return a-b;
}
static uchar _plus(uchar a, uchar b)
{
    int r = a+0+b;
    if (r>=255) return 255;
    return r;
}
static uchar _AmountMulti(uchar a, uchar b)
{
    int r = a*1*b/100;
    if (r>=255) return 255;
    return r;
}

TrBmp* TrSimpleSharp(TrBmp* src, int amount, int threshold)
{
    int i, j;
    if (!TrValidBmp(src)) return NULL;
    TrBmp* blur = TrNormalBlur(src);
    TrBmp* result = TrAllocBmp(src->width, src->height);
    {
        TrPixels* _dst = result->pixels;
        TrPixels* _src = src->pixels;
        TrPixels* _blur = blur->pixels;
        TrPixels temp;
        TrPixels mask={amount,amount,amount};
        for (i=0; i<src->width*src->height; ++i)
        {
            temp = TrPixelsBiOperate(*(_src+i), *(_blur+i), _minus);
            if (TrAbsPixels(temp)>=threshold)
            {
                *(_dst+i) = TrPixelsBiOperate(*(_src+i), TrPixelsBiOperate(temp, mask, _AmountMulti), _plus);
            }
            else
            {
                *(_dst+i) = *(_src+i);
            }
        }
    }
    TrFreeBmp(blur);
    return result;
}


TrBmp* TrSaturation(TrBmp* src, float satValue)
{
    float sat[] = {
        1,0,0,0,0,
        0,1,0,0,0,
        0,0,1,0,0,
        0,0,0,1,0
    };
    float R = 0.213f*(1-satValue);
    float G = 0.715f*(1-satValue);
    float B = 0.075f*(1-satValue);
    int i;
    sat[0] = R + satValue;
    sat[1] = G;
    sat[2] = B;

    sat[5] = R;
    sat[6] = G + satValue;
    sat[7] = B;

    sat[10] = R;
    sat[11] = G;
    sat[12] = B +satValue;
    TrBmp* dst = TrCopyAllocBmp(src);
    TrPixels* p = dst->pixels;
    for (i=0; i<dst->width*dst->height; ++i)
    {
        int oR = (p+i)->r;
        int oG = (p+i)->g;
        int oB = (p+i)->b;
        (p+i)->g = Tr_FLOAT_LIMIT_TO_CHAR(sat[5]*oR + sat[6]*oG + sat[7]*oB + +255*sat[8] + sat[9]);
        (p+i)->b = Tr_FLOAT_LIMIT_TO_CHAR(sat[10]*oR + sat[11]*oG + sat[12]*oB + 255*sat[13]+sat[14]);
        //(p+i)->b = Tr_FLOAT_LIMIT_TO_CHAR(sat[15]*oR + sat[16]*oG + sat[17]*oB + 255*sat[18]+sat[19]);
        (p+i)->r = Tr_FLOAT_LIMIT_TO_CHAR(sat[0]*oR + sat[1]*oG + sat[2]*oB + 255*sat[3]+sat[4]);
    }
    return dst;
}


