#include "regress/ALFloatMatrix.h"
#include "regress/ALUtils.h"
#define ZERO(x) ((x) > -0.000001 && (x) < 0.000001)

ALFloatMatrix::ALFloatMatrix(int w, int h):ALMatrix(w, h)
{
    GPASSERT(w>=0 && h>=0);
    int size = w*h;
    if (0 == size)
    {
        mBase = NULL;
    }
    mBase = new ALFLOAT[size];
}

ALFloatMatrix::~ALFloatMatrix()
{
    if (NULL!=mBase)
    {
        delete [] mBase;
    }
}

void ALFloatMatrix::vReset(int w, int h)
{
    if (NULL!=mBase)
    {
        delete mBase;
    }
    int size = w*h;
    if (0 == size)
    {
        mBase = NULL;
    }
    mBase = new ALFLOAT[size];
}

void ALFloatMatrix::vProduct(ALMatrix* A, ALMatrix* B)
{
    GPASSERT(A->width() == B->height());
    ALFloatMatrix* FA = dynamic_cast<ALFloatMatrix*>(A);
    ALFloatMatrix* FB = dynamic_cast<ALFloatMatrix*>(B);
    GPASSERT(NULL!=FA && NULL!=FB);
    int w = B->width();
    int h = A->height();
    int l = A->width();
    reset(w,h);
    int rA = A->vRowByte()/A->vUnit();
    int rB = B->vRowByte()/B->vUnit();
    int rC = this->vRowByte()/this->vUnit();
    ALFLOAT* a = (ALFLOAT*)A->vGetAddr();
    ALFLOAT* b = (ALFLOAT*)B->vGetAddr();
    ALFLOAT* c = mBase;
    for (int i=0; i<h; ++i)
    {
        for (int j=0; j<w; ++j)
        {
            ALFLOAT sum = 0;
            ALFLOAT* bs = b + j;
            ALFLOAT* as = a + i*rA;
            for (int k=0; k<l; ++k)
            {
                sum += (*as) * (*bs);
                as+=1;
                bs+=rB;
            }
            *(c + rC*i + j) = sum;
        }
    }
}
void ALFloatMatrix::vInverse(ALMatrix* A)
{
    ALFloatMatrix* FA = dynamic_cast<ALFloatMatrix*>(A);
    GPASSERT(NULL!=FA);
    GPASSERT(A->width() > 0 && A->height() > 0);
    int w = A->width();
    int h = A->height();
    GPASSERT(w == h);
    reset(w, w);
    ALFLOAT* a = new ALFLOAT[w*w];
    ALFLOAT* a_r = new ALFLOAT[w];
    ALFLOAT* c_r = new ALFLOAT[w];
    ALFLOAT* _a = (ALFLOAT*)A->vGetAddr();
    int rA = A->vRowByte()/A->vUnit();
    int rC = this->vRowByte()/this->vUnit();
    ALFLOAT* c = (ALFLOAT*)this->vGetAddr();
    for (int i=0; i<w; ++i)
    {
        for (int j=0; j<w; ++j)
        {
            *(a+i*w+j) = *(_a+i*rA+j);
            if (i==j)
            {
                *(c+rC*i+j) = 1;
            }
            else
            {
                *(c+rC*i+j) = 0;
            }
        }
    }
    int n = w;
    for (int i=0; i<n; ++i)
    {
        /*Swap until all M(i, i) is not zero*/
        bool zero = false;
        ALFLOAT weight = *(a+i*rA+i);
        if (ZERO(weight))
        {
            zero = true;
            for (int j=i+1; j<n; ++j)
            {
                ALFLOAT temp;
                weight = *(a+j*rA+i);
                if (!ZERO(weight))
                {
#define SWAP(x, y) temp=(x);(x)=(y);(y)=temp;
                    zero = false;
                    for (int r=i; r<n; ++r)
                    {
                        SWAP(*(a+i*rA+r), *(a+j*rA+r));
                        SWAP(*(c+i*rC+r), *(c+j*rC+r))/*Do the same for c*/
                    }
#undef SWAP
                }
            }
        }
        /*permit failed for inverse*/
        if (zero)
        {
            for (int j=0; j<n; ++j)
            {
                *(c+i+j*rC) = 0;
            }
            continue;
        }
        GPASSERT(!ZERO(weight));
        /*Do row transform to make the i row of A be "0 0 0 1 x1 x2...."*/
        //For A, the value before i column must be zero
        for (int j=i; j<n; ++j)
        {
            *(a+i*rA+j) /=weight;
            a_r[j] = *(a+i*rA+j);
        }
        //For C, it must do the same, but the value before i column may not be zero
        for (int j=0; j<n; ++j)
        {
            *(c+i*rC+j) /= weight;
            c_r[j] = *(c+i*rC+j);
        }
        /*Do row transform for other rows, make sure that all value in the column i zero*/
        for (int k=0; k<n; ++k)
        {
            if (k==i) continue;
            ALFLOAT p = *(a+k*rA+i);
            for (int j=i; j<n; ++j)
            {
                *(a+k*rA+j) -= (p*a_r[j]);
            }
            //The Same for c
            for (int j=0; j<n; ++j)
            {
                *(c+k*rC+j) -= (p*c_r[j]);
            }
        }
    }
    delete [] a;
    delete [] a_r;
    delete [] c_r;
}

void ALFloatMatrix::vTranspose(ALMatrix* A)
{
    ALFloatMatrix* FA = dynamic_cast<ALFloatMatrix*>(A);
    GPASSERT(NULL!=FA);
    GPASSERT(A->width() > 0 && A->height() > 0);
    int w = A->width();
    int h = A->height();
    reset(h, w);
    int rA = A->vRowByte()/A->vUnit();
    int rC = this->vRowByte()/this->vUnit();
    ALFLOAT* a = (ALFLOAT*)A->vGetAddr();
    ALFLOAT* c = (ALFLOAT*)this->vGetAddr();
    for (int i=0; i<h; ++i)
    {
        for (int j=0; j<w; ++j)
        {
            *(c+j*rC+i) = *(a+i*rA+j);
        }
    }
}

void ALFloatMatrix::vSTS(ALMatrix* A, bool transpose)
{
    ALFloatMatrix* FA = dynamic_cast<ALFloatMatrix*>(A);
    GPASSERT(NULL!=FA);
    GPASSERT(A->width() > 0 && A->height() > 0);
    int w = A->width();
    int h = A->height();
    ALFLOAT* a = (ALFLOAT*)A->vGetAddr();
    ALFLOAT* b = a;
    int rA = A->vRowByte()/A->vUnit();
    int ws, hs;
    if (!transpose)
    {
        ws = 1;
        hs = rA;
    }
    else
    {
        ws = rA;
        hs = 1;
        int temp = w;
        w = h;
        h = temp;
    }
    reset(w,w);
    int rC = this->vRowByte()/this->vUnit();
    ALFLOAT* c = mBase;
    /*Compute the up triangle*/
    for (int i=0; i<w; ++i)
    {
        for (int j=i; j<w; ++j)
        {
            ALFLOAT sum = 0; 
            ALFLOAT* sA = a + i*ws;
            ALFLOAT* sB = b + j*ws;
            for (int k=0; k<h; ++k)
            {
                sum += (*sA)*(*sB);
                sA += hs;
                sB += hs;
            }
            *(c+i*rC+j) = sum;
        }
    }
    /*Complete the matrix*/
    for (int i=0; i<w; ++i)
    {
        for (int j=0; j<i; ++j)
        {
            *(c+i*rC+j) = *(c+j*rC+i);
        }
    }
}

void ALFloatMatrix::vPrint(std::ostream& os)
{
    int rC = this->vRowByte()/this->vUnit();
    for (int i=0; i<height(); ++i)
    {
        for (int j=0; j<width(); ++j)
        {
            os << *(mBase+i*rC+j) <<"    ";
        }
        os << "\n";
    }
}
