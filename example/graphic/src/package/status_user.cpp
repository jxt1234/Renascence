/*AUTO-GENERATE*/
#include "status_user.h"
#include "debug.h"
#include <sstream>
#include <iostream>
#include <string>
extern "C"
{
#include "transform_high.h"
#include "regress/graphic_regress.h"
}
#include <stdlib.h>
#include <string.h>

#define MAX_GRAPHIC_NUM 2
#define DEFAULT_DIM 3
class TrFilterMatrixType:public IStatusType
{
    public:
        TrFilterMatrixType():IStatusType("TrFilterMatrix"){}
        virtual void* vLoad(std::istream& input) const
        {
            //std::cout << src<<std::endl;
            std::istream& inp = input;
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
        virtual void vSave(void* contents, std::ostream& output) const
        {
            std::ostream& res = output;
            TrFilterMatrix* m = (TrFilterMatrix*)contents;
            res << m->dim<<" "<<m->offset<<" ";
            for (int i=0; i<(m->dim)*(m->dim); ++i)
            {
                res << m->data[i]<<" ";
            }
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
            for(int i=0; i < DEFAULT_DIM*DEFAULT_DIM; ++i)
            {
                p[i] = value[i];
            }
            return DEFAULT_DIM*DEFAULT_DIM;
        }
};

class TrFloatType:public IStatusType
{
    public:
        TrFloatType():IStatusType("float"){}
        virtual void* vLoad(std::istream& input) const
        {
            float* f = (float*)malloc(sizeof(float)*MAX_GRAPHIC_NUM);
            for (int i=0; i<MAX_GRAPHIC_NUM; ++i)
            {
                input >> f[i];
            }
            return (void*)f;
        }
        virtual void vSave(void* contents, std::ostream& output) const
        {
            float* f = (float*)contents;
            for (int i=0; i<MAX_GRAPHIC_NUM; ++i)
            {
                output << f[i] <<" ";
            }
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
        virtual void* vLoad(std::istream& src) const
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
        virtual void vSave(void* src, std::ostream& out) const
        {
            TrRegreeMode* m = (TrRegreeMode*)src;
            out << m->dim<<" ";
            for (int i=0; i<m->dim*m->dim; ++i)
            {
                out << m->mode[i]<<" ";
            }
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
        virtual void* vLoad(std::istream& input) const
        {
            std::string s;
            input >> s;
            return TrLoadPixels(s.c_str());
        }
        virtual void vSave(void* contents, std::ostream& output) const
        {
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
        virtual void* vLoad(std::istream& input) const
        {
            return NULL;
        }
        virtual void vSave(void* contents, std::ostream& output) const
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


IStatusType* GP_IStatusType_Create(const std::string& name)
{
    if (name == "float")
    {
        return new TrFloatType();
    }
    if (name == "TrFilterMatrix")
    {
        return new TrFilterMatrixType();
    }
    if (name == "TrRegreeMode")
    {
        return new TrRegreeModeType();
    }
    if (name == "TrBmp")
    {
        return new TrBmpType();
    }
    if (name == "double")
    {
        return new TrDoubleType();
    }
    return NULL;
}
