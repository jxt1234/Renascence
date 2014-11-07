extern "C"
{
#include "regress/graphic_regress.h"
}
#include <utils/debug.h>
#include <string.h>
#include "debug.h"

#include "regress/ALFloatMatrix.h"

static int getRegressNumber(TrRegreeMode* mode)
{
    int i, j;
    int selectNumber = 0;
    int dim = mode->dim;
    GPASSERT(NULL!=mode);
    for(i=0; i<dim; ++i)
    {
        for (j=0; j<dim; ++j)
        {
            if (1 == (mode->mode)[i*dim+j])
            {
                selectNumber++;
            }
        }
    }
    return selectNumber;
}

void TrRegressExpand(TrBmp* src, TrRegreeMode* mode, ALFloatMatrix* M, int hasConst)
{
    GPASSERT(TrValidBmp(src));
    GPASSERT(NULL!=mode);
#define FILTER_EXPAND(x, y) (x-(mode->dim)/2)+(y-(mode->dim)/2)*(src->width)
    int i,j,k;
    int selectNumber, allSelected=0;
    int* select;
    int dim = mode->dim;
    const int staPos = dim/2;
    const int lossPixels = (dim/2)*2;
    const int width = src->width-lossPixels;
    const int height = src->height -lossPixels;
    int area = width*height;
    TrPixels* srcBase = src->pixels + staPos*src->width+staPos;
    select = (int*)malloc(sizeof(int)*dim*dim);
    GPASSERT(NULL!=select);
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
    //Do Expanding
    float* X = (float*)M->vGetAddr();
    int stride = M->vRowByte()/M->vUnit();
    for (i=0; i<height; ++i)
    {
        for (j=0; j<width; ++j)
        {
            for (k=0; k<selectNumber; ++k)
            {
                TrPixels* p = srcBase+(i*src->width+j+ select[k]);
                *(X+(i*width+j)*stride+k) = (float)((int)p->r + (int)p->b +(int)p->g)/3.0;
            }
        }
    }
    if (hasConst)
    {
        //Add Const column, which is 1.0 for all
        for (i=0; i<area; ++i)
        {
            *(X+i*stride+stride-1) = 1.0;
        }
    }
    free(select);
#undef FILTER_EXPAND
}

int TrRegreeModeNumberCompute(TrRegreeMode* mode)
{
    GPASSERT(NULL!=mode);
    GPASSERT(NULL!=mode->mode);
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
    GPASSERT(TrValidBmp(src));
    GPASSERT(TrValidBmp(target));
    GPASSERT(mode);
    GPASSERT(src->width*src->height == target->width*target->height);
    TrFilterMatrix* result;
    TrRegreeMode* oriMode;
    const int lossPixels = (mode->dim/2)*2;
    int area = (src->height-lossPixels)*(src->width-lossPixels);
    int i;
    const int dim = mode->dim;
    result = TrFilterMatrixAlloc(mode->dim);
    /*Expand*/
    oriMode = TrRegreeSingleModeAlloc(mode->dim);
    int x_w = getRegressNumber(mode);
    ALFloatMatrix* X = new ALFloatMatrix(x_w+1, area);//Add constant
    ALFloatMatrix* Y = new ALFloatMatrix(1, area);
    TrRegressExpand(target, oriMode, Y, 0);
    TrRegressExpand(src, mode, X, 1);
    //Compute
    ALMatrix* XT = new ALFloatMatrix;
    XT->vTranspose(X);
    delete X;//Not used in future
    ALMatrix* XTX=new ALFloatMatrix;
    XTX->vSTS(XT, true);
    /*Result = (XTX)-1 * XT * Y, P, Q for mid compute*/
    ALMatrix* P = new ALFloatMatrix;
    ALMatrix* Q = new ALFloatMatrix;
    P->vInverse(XTX);
    Q->vProduct(P, XT);
    P->vProduct(Q, Y);
    //P is the result, delete other value
    delete Q;
    delete Y;
    delete XTX;
    delete XT;
    //Load value from parameters
    {
        float* parameters = (float*)P->vGetAddr();
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
    delete P;

    TrRegreeModeFree(oriMode);

    return result;
}
