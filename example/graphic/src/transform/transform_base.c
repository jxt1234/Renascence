#include "transform_high.h"
#include "debug.h"
#include <string.h>

TrFilterData* TrFilterDataAlloc(int number)
{
    TrFilterData* result  = (TrFilterData*)malloc(sizeof(TrFilterData));
    result->data = (TrPixels**)malloc(sizeof(TrPixels*)*number);
    return result;
}

void TrFilterDataFree(TrFilterData* data)
{
    if (NULL!=data)
    {
        free(data->data);
        free(data);
    }
}
void TrFilterMatrixFree(TrFilterMatrix* m)
{
    if (NULL!=m)
    {
        free(m->data);
        free(m);
    }
}

char* TrFilterMatrixPrint(TrFilterMatrix* m)
{
    char* result = NULL;
    if (NULL!=m)
    {
        int i;
        int cur;
        int length = m->dim*m->dim;
        int size = m->dim*m->dim*5/*data*/ + 2/*dim*/ + 3/*offset*/ + 100/*Other text*/;
        result = (char*)malloc(sizeof(char)*size);
        memset(result, size*sizeof(char), ' ');
        cur = sprintf(result, "dim = %d, offset = %d \n", m->dim, m->offset);
        for (i=0; i<length; ++i)
        {
            result[cur] = ' ';
            cur = cur + sprintf(result+cur+1, " %0.5f ", (m->data)[i]);
        }
        result[cur+1] = '\0';
    }
    return result;
}

TrFilterMatrix* TrFilterMatrixAlloc(int dim)
{
    TrFilterMatrix* result;
    result = (TrFilterMatrix*)malloc(sizeof(TrFilterMatrix));
    result->dim = dim;
    result->data = (float*)malloc(sizeof(float)*dim*dim);
    result->offset = 0;
    return result;
}
TrFilterMatrix* TrFilterMatrixCopyAlloc(float* src, int dim)
{
    int i;
    TrFilterMatrix* result = TrFilterMatrixAlloc(dim);
    for (i=0; i<dim*dim; ++i)
    {
        *(result->data+i) = src[i];
    }
    result->offset = 0;
    return result;
}
