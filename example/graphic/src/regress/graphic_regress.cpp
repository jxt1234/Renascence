extern "C"
{
#include "regress/graphic_regress.h"
}
#include <assert.h>
#include <string.h>
#include "debug.h"

void TrRegressExpand(TrBmp* src, TrRegreeMode* mode, float* X, integer* M, integer* N, int hasConst)
{
    assert(TrValidBmp(src));
    assert(NULL!=mode);
#define FILTER_EXPAND(x, y) (x-(mode->dim)/2)+(y-(mode->dim)/2)*(src->width)
    int i,j,k;
    int selectNumber, allSelected=0;
    int* select;
    int dim = mode->dim;
    const int staPos = dim/2;
    const int lossPixels = (dim/2)*2;
    const int width = src->width-lossPixels;
    const int height = src->height -lossPixels;
    TrPixels* srcBase = src->pixels + staPos*src->width+staPos;
    select = (int*)malloc(sizeof(int)*dim*dim);
    assert(NULL!=select);
    *M = width*height;
    /*Determine which value is used*/
    selectNumber = 0;
    for(i=0; i<dim; ++i)
    {
        for (j=0; j<dim; ++j)
        {
            if (1 == (mode->mode)[i*dim+j])
            {
                select[selectNumber++] = FILTER_EXPAND(i, j);
            }
        }
    }
    *N=selectNumber+hasConst;
    //Do Expanding
    assert(NULL!=X);
    for (k=0; k<selectNumber; ++k)
    {
        for (i=0; i<height; ++i)
        {
            for (j=0; j<width; ++j)
            {
                TrPixels* p = srcBase+(i*src->width+j+ select[k]);
                *(X+(i*width+j)+(width*height*k)) = (float)((int)p->r + (int)p->b +(int)p->g)/3.0;
            }
        }
    }
    if (hasConst)
    {
        //Add Const column, which is 1.0 for all
        for (i=0; i<height*width; ++i)
        {
            *(X+i+k*height*width) = 1.0;
        }
    }
    free(select);
#undef FILTER_EXPAND
}

int TrRegreeModeNumberCompute(TrRegreeMode* mode)
{
    assert(NULL!=mode);
    assert(NULL!=mode->mode);
    int i, result=0;
    for (i=0; i<mode->dim*mode->dim; ++i)
    {
        if (1 == mode->mode[i])
        {
            result++;
        }
    }
    return result;
}

TrRegreeMode* TrRegreeSingleModeAlloc(int dim)
{
    int i;
    TrRegreeMode* mode = (TrRegreeMode*)malloc(sizeof(TrRegreeMode));
    mode->dim = dim;
    mode->mode = (int*)malloc(dim*dim*sizeof(int));
    for (i=0; i<dim*dim; ++i)
    {
        (mode->mode)[i] = 0;
    }
    (mode->mode)[dim*(dim/2)+(dim/2)]=1.0;
    return mode;
}

TrRegreeMode* TrRegreeModeAlloc(int dim)
{
    int i;
    TrRegreeMode* mode = (TrRegreeMode*)malloc(sizeof(TrRegreeMode));
    mode->dim = dim;
    mode->mode = (int*)malloc(sizeof(int)*dim*dim);
    for (i=0; i<dim*dim; ++i)
    {
        (mode->mode)[i] = 1;
    }
    return mode;
}
void TrRegreeModeFree(TrRegreeMode* mode)
{
    if (NULL!=mode->mode)
    {
        free(mode->mode);
    }
    free(mode);
}
void TrRegreeModeCopy(TrRegreeMode* src, TrRegreeMode* dst)
{
    int size = sizeof(int)*src->dim*src->dim;
    if (dst->dim !=src->dim)
    {
        if (dst->mode!=NULL)
        {
            free(dst->mode);
        }
        dst->mode = (int*)malloc(size);
    }
    dst->dim = src->dim;
    memcpy(dst->mode, src->mode, size);
}

TrFilterMatrix* TrRegressMatrix(TrBmp* src, TrBmp* target, TrRegreeMode* mode)
{
    assert(TrValidBmp(src));
    assert(TrValidBmp(target));
    assert(mode);
    assert(src->width*src->height == target->width*target->height);
    TrFilterMatrix* result;
    TrRegreeMode* oriMode;
    int area = src->height*src->width;
    int i;
    const int dim = mode->dim;
    //For lapack
    float* parameters;
    float* X;
    float* Y;
    float* WORK;
    integer M, N, P=0;
    integer LDB = 1;
    integer LWORK;
    integer INFO;

    result = TrFilterMatrixAlloc(mode->dim);
    /*Expand*/
    oriMode = TrRegreeSingleModeAlloc(mode->dim);
    Y = (float*)malloc(area*sizeof(float));
    assert(NULL!=Y);
    X = (float*)malloc(area*sizeof(float)*(TrRegreeModeNumberCompute(mode)+1));
    assert(NULL!=X);
    TrRegressExpand(target, oriMode, Y, &M, &N, 0);
    TrRegressExpand(src, mode, X, &M, &N, 1);
    parameters = (float*)malloc(sizeof(float)*(N));
    /*Run regress*/
    LWORK = M+N+P;
    WORK = (float*)malloc(sizeof(float)*LWORK);
    assert(NULL!=WORK);
    sgglse_(&M, &N, &P, X, &M, NULL, &LDB, Y, NULL, parameters, WORK, &LWORK, &INFO);
    //Load value from parameters
    {
        int* mod = mode->mode;
        float* matrix = result->data;
        int cur = 0;
        for (i=0; i<dim*dim; ++i)
        {
            if (1==mod[i])
            {
                matrix[i] = parameters[cur++];
            }
        }
        //The last parameters is offset
        result->offset = (int)parameters[cur];
    }

    TrRegreeModeFree(oriMode);
    //Free lapack parameters
    free(parameters);
    free(X);
    free(Y);
    free(WORK);

    return result;
}
void TrRegressMixFactor(TrBmp* dst, TrBmp** src, int inputNumber, float* parameters)
{
    assert(NULL!=src);
    assert(NULL!=dst);
    assert(NULL!=parameters);
    int i;
    int area = dst->width*dst->height;
    for (i=0; i<inputNumber; ++i)
    {
        TrBmp* src_u = src[i];
        assert(NULL!=src_u);
        assert(area == src_u->width * src_u->height);
    }
    /*Allocate memory*/
    TrRegreeMode* mode = TrRegreeModeAlloc(1);
    float* X = (float*)malloc(sizeof(float)*area*inputNumber);
    assert(NULL!=X);
    float* Y = (float*)malloc(sizeof(float)*area);
    assert(NULL!=Y);
    integer M,N,LWORK,LDB=1,INFO, P=0;
    float* WORK;
    TrRegressExpand(dst, mode, Y, &M, &N, 0);
    for (i=0; i<inputNumber; ++i)
    {
        TrRegressExpand(src[i], mode, X+i*area, &M, &N, 0);
    }
    N = inputNumber;
    /*Run regress*/
    LWORK = M+N+P;
    WORK = (float*)malloc(sizeof(float)*LWORK);
    assert(NULL!=WORK);
    sgglse_(&M, &N, &P, X, &M, NULL, &LDB, Y, NULL, parameters, WORK, &LWORK, &INFO);
    /*Free all mid memory*/
    free(X);
    free(Y);
    free(WORK);
    TrRegreeModeFree(mode);
}
