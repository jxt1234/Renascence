/*AUTO-GENERATE*/
#include <string>
#include <sstream>
#include <iostream>
#include <string>
#include "debug.h"
#include <string.h>
#define MAX_GRAPHIC_NUM 2
#define DEFAULT_DIM 3
extern "C"
{
#include "transform_high.h"
#include "regress/graphic_regress.h"
}
#include <stdlib.h>
#include "user/IStatusType.h"
class TrFilterMatrixType:public IStatusType
{
    public:
        TrFilterMatrixType():IStatusType("TrFilterMatrix"){}
        virtual void* vLoad(GPStream* input) const
        {
            GPStream* inp = input;
            int dim = inp->readUnit<int>();
            TrFilterMatrix* m = TrFilterMatrixAlloc(dim);
            m->offset = inp->readUnit<int>();
            for (int i=0; i<dim*dim; ++i)
            {
                m->data[i] = inp->readUnit<float>();
            }
            return (void*)m;
        }
        virtual void vSave(void* contents, GPWStream* output) const
        {
            std::ostringstream res;
            TrFilterMatrix* m = (TrFilterMatrix*)contents;
            res << m->dim<<" "<<m->offset<<" ";
            for (int i=0; i<(m->dim)*(m->dim); ++i)
            {
                res << m->data[i]<<" ";
            }
            output->write(res.str().c_str(), res.str().size());
        }
        virtual void vFree(void* content) const
        {
            TrFilterMatrixFree((TrFilterMatrix*)content);
        }
        virtual int vMap(void** content, double* value) const
        {
            if (NULL==value || NULL==content)
            {
                return DEFAULT_DIM*DEFAULT_DIM;
            }
            if (NULL == *content)
            {
                *content = (void*)TrFilterMatrixAlloc(DEFAULT_DIM);
            }
            TrFilterMatrix* m = (TrFilterMatrix*)(*content);
            float* p = m->data;
            float sum = 0.0;
            for(int i=0; i < DEFAULT_DIM*DEFAULT_DIM; ++i)
            {
                p[i] = value[i];
                sum+=value[i];
            }
            for (int i=0; i < DEFAULT_DIM*DEFAULT_DIM; ++i)
            {
                p[i] /= sum;
            }
            return DEFAULT_DIM*DEFAULT_DIM;
        }
};

class TrFloatType:public IStatusType
{
    public:
        TrFloatType():IStatusType("float"){}
        virtual void* vLoad(GPStream* input) const
        {
            return NULL;
        }
        virtual void vSave(void* contents, GPWStream* out) const
        {
            std::ostringstream output;
            float* f = (float*)contents;
            for (int i=0; i<MAX_GRAPHIC_NUM; ++i)
            {
                output << f[i] <<" ";
            }
            out->write(output.str().c_str(), output.str().size());
        }
        virtual void vFree(void* contents) const
        {
            free(contents);
        }
        virtual int vMap(void** content, double* value) const
        {
            int mapnumber = MAX_GRAPHIC_NUM-1;
            if (NULL == value || NULL == content)
            {
                return mapnumber;
            }
            if (NULL == *content)
            {
                *content = malloc(sizeof(float)*MAX_GRAPHIC_NUM);
            }
            float* v = (float*)(*content);
            float sum = 1.0;
            for (int i=0; i<mapnumber; ++i)
            {
                v[i] = value[i];
                sum -= v[i];
            }
            v[mapnumber] = sum;
            return mapnumber;
        }
};

class TrRegreeModeType:public IStatusType
{
    public:
        TrRegreeModeType():IStatusType("TrRegreeMode"){}
        virtual void* vLoad(GPStream* src) const
        {
            return NULL;
        }
        virtual void vSave(void* src, GPWStream* output) const
        {
            std::ostringstream out;
            TrRegreeMode* m = (TrRegreeMode*)src;
            out << m->dim<<" ";
            for (int i=0; i<m->dim*m->dim; ++i)
            {
                out << m->mode[i]<<" ";
            }
            output->write(out.str().c_str(), out.str().size());
        }
        virtual void vFree(void* contents) const
        {
            TrRegreeMode* mode = (TrRegreeMode*)contents;
            TrRegreeModeFree(mode);
        }
        virtual int vMap(void** content, double* value) const
        {
            int mapnumber = DEFAULT_DIM*DEFAULT_DIM;
            if (NULL == value || NULL == content)
            {
                return mapnumber;
            }
            if (NULL == *content)
            {
                *content = (void*)TrRegreeModeAlloc(DEFAULT_DIM);
            }
            TrRegreeMode* mode = (TrRegreeMode*)(*content);
            int* data = mode->mode;
            for (int i=0; i<mapnumber; ++i)
            {
                if (value[i]>=0.5)
                {
                    data[i] = 1;
                }
                else
                {
                    data[i] = 0;
                }
            }
            return mapnumber;
        }
};

class TrBmpType:public IStatusType
{
    public:
        TrBmpType():IStatusType("TrBmp"){}
        virtual void* vLoad(GPStream* input) const
        {
            /*TODO*/
            return NULL;
        }
        virtual void vSave(void* contents, GPWStream* output) const
        {
            /*TODO*/
            return;
        }
        virtual void vFree(void* contents) const
        {
            TrFreeBmp((TrBmp*)contents);
        }
        virtual int vMap(void** contents, double* value) const
        {
            return 0;
        }
};

class TrDoubleType:public IStatusType
{
    public:
        TrDoubleType():IStatusType("double"){}
        virtual void* vLoad(GPStream* input) const
        {
            return NULL;
        }
        virtual void vSave(void* contents, GPWStream* output) const
        {
        }
        virtual void vFree(void* contents) const
        {
            delete (double*)contents;
        }
        virtual int vMap(void** content, double* value) const
        {
            return 0;
        }
};


extern TrFloatType gFloatType;
extern TrFilterMatrixType gTrFilterMatrixType;
extern TrRegreeModeType gTrRegreeModeType;
extern TrBmpType gTrBmpType;
extern TrDoubleType gTrDoubleType;
extern "C"{
IStatusType* GP_IStatusType_Create(const std::string& name);
}
