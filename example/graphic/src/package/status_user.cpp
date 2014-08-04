/*AUTO-GENERATE*/
#include "status_user.h"
#include "debug.h"
#include <sstream>
#include <iostream>
extern "C"
{
#include "transform_high.h"
#include "regress/graphic_regress.h"
}
#include <stdlib.h>
#include <string.h>

#define MAX_GRAPHIC_NUM 2
#define DEFAULT_DIM 3
inline void unitRandom(float& num)
{
    num+= (rand()%1000-500)/4000.0;
    if (num>1) num-=1;
    if (num<0) num+=1;
}

inline void unitRandom(int& num)
{
    num = (rand()%5)%2;
}

void* TrFilterMatrix_alloc()
{
    float src[]={0.1, 0.1, 0.1, 0.1, 0.2, 0.1, 0.1, 0.1, 0.1};
    TrFilterMatrix* m = TrFilterMatrixCopyAlloc(src, DEFAULT_DIM);
    return (void*)m;
}

void TrFilterMatrix_free(void* src)
{
    TrFilterMatrix* m = (TrFilterMatrix*)src;
    TrFilterMatrixFree(m);
}

void TrFilterMatrix_vary(void* src)
{
    TrFilterMatrix* m = (TrFilterMatrix*)src;
    float* p = m->data;
    for(int i=0; i < DEFAULT_DIM*DEFAULT_DIM; ++i)
    {
        unitRandom(p[i]);
    }
}

void TrFilterMatrix_copy(void* src, void* dst)
{
    TrFilterMatrix* s = (TrFilterMatrix*)src;
    TrFilterMatrix* d = (TrFilterMatrix*)dst;
    d->dim = s->dim;
    d->offset = s->offset;
    memcpy(d->data, s->data, sizeof(float)*DEFAULT_DIM*DEFAULT_DIM);
}

void TrFilterMatrix_print(std::ostream& out, void* src)
{
    std::ostream& res = out;
    TrFilterMatrix* m = (TrFilterMatrix*)src;
    res << m->dim<<" "<<m->offset<<" ";
    for (int i=0; i<(m->dim)*(m->dim); ++i)
    {
        res << m->data[i]<<" ";
    }
}

void* TrFilterMatrix_load(std::istream& src)
{
    //std::cout << src<<std::endl;
    std::istream& inp = src;
    int dim;
    inp>>dim;
    TrFilterMatrix* m = TrFilterMatrixAlloc(dim);
    inp>>m->offset;
    for (int i=0; i<dim*dim; ++i)
    {
        inp >> m->data[i];
        //std::cout << m->data[i]<<"\t";
    }
    //std::cout << std::endl;
    return (void*)m;
}

void* float_alloc()
{
    float* src = (float*)malloc(sizeof(float)*MAX_GRAPHIC_NUM);
    for (int i=0; i<MAX_GRAPHIC_NUM;++i)
    {
        src[i] = 1.0/MAX_GRAPHIC_NUM;
    }
    return (void*)src;
}

void float_free(void* src)
{
    float* s = (float*)src;
    free(s);
}

void float_vary(void* src)
{
    float* data = (float*)src;
    for (int i=0; i<MAX_GRAPHIC_NUM; ++i)
    {
        unitRandom(data[i]);
    }
}

void float_copy(void* src, void* dst)
{
    memcpy(dst, src, sizeof(float)*MAX_GRAPHIC_NUM);
}

void float_print(std::ostream& out, void* src)
{
    float* f = (float*)src;
    for (int i=0; i<MAX_GRAPHIC_NUM; ++i)
    {
        out << f[i] <<" ";
    }
}

void* float_load(std::istream& src)
{
    float* f = (float*)malloc(sizeof(float)*MAX_GRAPHIC_NUM);
    for (int i=0; i<MAX_GRAPHIC_NUM; ++i)
    {
        src >> f[i];
    }
    return (void*)f;
}

void* TrRegreeMode_alloc()
{
    TrRegreeMode* mode = TrRegreeModeAlloc(DEFAULT_DIM);
}

void TrRegreeMode_free(void* src)
{
    TrRegreeMode* mode = (TrRegreeMode*)src;
    TrRegreeModeFree(mode);
}

void TrRegreeMode_vary(void* src)
{
    TrRegreeMode* mode = (TrRegreeMode*)src;
    int* data = mode->mode;
    for (int i=0; i<mode->dim*mode->dim; ++i)
    {
        unitRandom(data[i]);
    }
}

void TrRegreeMode_copy(void* src, void* dst)
{
    TrRegreeMode* s = (TrRegreeMode*)src;
    TrRegreeMode* d = (TrRegreeMode*)dst;
    TrRegreeModeCopy(s,d);
}

void TrRegreeMode_print(std::ostream& out, void* src)
{
    TrRegreeMode* m = (TrRegreeMode*)src;
    out << m->dim<<" ";
    for (int i=0; i<m->dim*m->dim; ++i)
    {
        out << m->mode[i]<<" ";
    }
}

void* TrRegreeMode_load(std::istream& src)
{
    int dim;
    src >> dim;
    TrRegreeMode* mode = TrRegreeModeAlloc(dim);
    for(int i=0; i<dim*dim; ++i)
    {
        src >> mode->mode[i];
    }
    return (void*)mode;
}
