#ifndef INCLUDE_CORE_ALFLOATMATRIX_H
#define INCLUDE_CORE_ALFLOATMATRIX_H
#include "ALMatrix.h"
typedef float ALFLOAT;
class ALFloatMatrix:public ALMatrix
{
    public:
        ALFloatMatrix(int w = 0, int h = 0);
        virtual ~ALFloatMatrix();
        virtual void* vGetAddr() const{return (void*)mBase;}
        virtual int vUnit() const{return sizeof(ALFLOAT);}
        virtual void vProduct(ALMatrix* A, ALMatrix* B);
        virtual void vInverse(ALMatrix* A);
        virtual void vTranspose(ALMatrix* A);
        virtual void vSTS(ALMatrix* A, bool transpose = false);
        virtual void vPrint(std::ostream& os);
    protected:
        virtual void vReset(int w, int h);
        ALFLOAT* mBase;
};
#endif
