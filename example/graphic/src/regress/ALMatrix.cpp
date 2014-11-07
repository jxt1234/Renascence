#include "regress/ALMatrix.h"
#include "regress/ALUtils.h"
void ALMatrix::reset(int w, int h)
{
    GPASSERT(0 < w && 0 < h);
    if (mW != w || mH != h)
    {
        mW = w;
        mH = h;
        this->vReset(w, h);
    }
}
