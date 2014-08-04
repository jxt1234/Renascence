#ifndef INCLUDE_CORE_ALMATRIX_H
#define INCLUDE_CORE_ALMATRIX_H
#include "ALRefCount.h"
#include <ostream>
/*This is just for continue matirx*/
class ALMatrix:public ALRefCount
{
    public:
        ALMatrix(int w, int h):mW(w), mH(h){}
        virtual ~ALMatrix(){}
        void reset(int w, int h);
        inline int width() const{return mW;}
        inline int height() const{return mH;}
        /*To inherit*/
        virtual void* vGetAddr() const = 0;
        /*If the memory is alloc by gpu(opencl) or dsp, the rowByte will not be the same as unitSize* w*/
        virtual int vRowByte() const {return mW*this->vUnit();}
        /*Return the size of data, such as sizeof(float)*/
        virtual int vUnit() const = 0;
        /*Operator*/
        /*this = A * B*/
        virtual void vProduct(ALMatrix* A, ALMatrix* B) = 0;
        /*this = A-1*/
        virtual void vInverse(ALMatrix* A) {}
        /*this = A'*/
        virtual void vTranspose(ALMatrix* A){}
        /*this = A'*A', used for regress*/
        virtual void vSTS(ALMatrix* A, bool transpose = false){}
        virtual void vPrint(std::ostream& os) {}
    protected:
        virtual void vReset(int w, int h) = 0;
    private:
        int mW;
        int mH;
};
#endif
