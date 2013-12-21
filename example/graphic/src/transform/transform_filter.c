#include "transform_high.h"
#include <math.h>

TrBmp* TrFilterTransform(TrBmp* src, TrLoadFunc load, TrReduceFunc reduce)
{
    if (!TrValidBmp(src)) return NULL;
    TrBmp* dst = TrAllocBmp(src->width, src->height);
    int i, j;
    for(i=0; i<src->width; ++i)
    {
        for (j=0; j<src->height; ++j)
        {
            *(dst->pixels+j*src->width+i) = reduce(load(src, i, j));
        }
    }
    return dst;
}

static TrFilterData* _load_2(TrBmp* src, int x, int y)
{
    TrFilterData* result = TrFilterDataAlloc(2);
    result->data[0] = src->pixels+y*src->width+x;
    x--;
    if (x<0) x = 0;
    result->data[1] = src->pixels+y*src->width+x;
    return result;
}

static TrPixels _minus_2(TrFilterData* data)
{
    TrPixels result;
    TrPixels** pixels = data->data;
    result.r = pixels[0]->r - pixels[1] -> r + 128;
    result.g = pixels[0]->g - pixels[1] -> g + 128;
    result.b = pixels[0]->b - pixels[1] -> b + 128;
    TrFilterDataFree(data);
    return result;
}

TrBmp* TrFilterRelief(TrBmp* src)
{
    return TrFilterTransform(src, _load_2, _minus_2);
}



static TrFilterData* _randinBowLoad(TrBmp* src, int x, int y)
{
    TrFilterData* result = TrFilterDataAlloc(3);
    result->data[0] = src->pixels + y*src->width + x;
    result->data[1] = src->pixels + ((y+1)%src->height)*src->width + x;
    result->data[2] = src->pixels + y*src->width + (x+1)%(src->width);
    return result;
}

#define DISTANCE_RAIN(r1, r2, r3, c) \
    2*sqrt((r1->c-r2->c)*(r1->c-r2->c) + (r1->c-r3->c)*(r1->c-r3->c))

static TrPixels _randinBowReduce(TrFilterData* data)
{
    TrPixels result;
    TrPixels** pixels = data->data;
    result.r = DISTANCE_RAIN(pixels[0], pixels[1], pixels[2], r);
    result.g = DISTANCE_RAIN(pixels[0], pixels[1], pixels[2], g);
    result.b = DISTANCE_RAIN(pixels[0], pixels[1], pixels[2], b);
    TrFilterDataFree(data);
    return result;
}


TrBmp* TrFilterRainbow(TrBmp* src)
{
    if (!TrValidBmp(src)) return NULL;
    return TrFilterTransform(src, _randinBowLoad, _randinBowReduce);
}


TrPixels TrFilterMatrixCompute(TrBmp* src, int x, int y, TrFilterMatrix* modle)
{
    int i,j;
    int cur_x,cur_y;
    TrPixels result;
    TrPixels* cur;
    float r=0;
    float g=0;
    float b=0;
    int dim = modle->dim;
    for (i=0; i<modle->dim; ++i)
    {
        cur_x = x-modle->dim/2+i;
        Tr_LIMIT(cur_x, src->width);
        for (j=0; j<modle->dim; ++j)
        {
            cur_y = y-modle->dim/2+j;
            Tr_LIMIT(cur_y, src->height);
            cur = src->pixels+cur_y*src->width + cur_x;
            r += (*(modle->data+j*dim+i)*cur->r);
            g += (*(modle->data+j*dim+i)*cur->g);
            b += (*(modle->data+j*dim+i)*cur->b);
        }
    }
    result.r = Tr_FLOAT_LIMIT_TO_CHAR((r+modle->offset));
    result.g = Tr_FLOAT_LIMIT_TO_CHAR((g+modle->offset));
    result.b = Tr_FLOAT_LIMIT_TO_CHAR((b+modle->offset));
    return result;
}

TrBmp* TrFilterMatrixTransform(TrBmp* src, TrFilterMatrix* matrix)
{
    if (!TrValidBmp(src)) return NULL;
    if (NULL==matrix) return NULL;
    TrBmp* dst = TrAllocBmp(src->width, src->height);
    int i, j;
    for (i=0; i<src->width; ++i)
    {
        for (j=0; j<src->height; ++j)
        {
            *(dst->pixels+j*src->width+i) = TrFilterMatrixCompute(src, i, j, matrix);
        }
    }
    return dst;
}
