#include "gppackage.h"
#include "status_user.h"
#include <assert.h>
using namespace std;
GPContents* TrPackageFilterMatrix(GPContents* inputs)
{
    assert(inputs->size() == 2);
    GPContents* result = new GPContents;
    TrBmp* dst = TrFilterMatrixTransform((TrBmp*)inputs->get(0), (TrFilterMatrix*)inputs->get(1));
    result->push(dst, &gTrBmpType);
    return result;
}

GPContents* TrPackageSaturation(GPContents* inputs)
{
    assert(inputs->size() == 2);
    GPContents* result = new GPContents;
    TrBmp* dst = TrSaturation((TrBmp*)inputs->get(0), 2*((float*)inputs->get(1))[0]);
    result->push(dst, &gTrBmpType);
    return result;
}

//以第一幅图像的大小为最终的图像大小
GPContents* TrPackageCompse(GPContents* inputs)
{
    assert(inputs->size() >= 2);
    GPContents* result = new GPContents;
    int picsize = inputs->size() - 1;
    TrBmp** pic = (TrBmp**)(malloc(picsize*sizeof(TrBmp)));
    float* factor = (float*)inputs->get(picsize);
    for (int i=0; i<picsize; ++i)
    {
        pic[i] = (TrBmp*)inputs->get(i);
    }
    TrBmp* dst = TrMixPicture(pic, factor, picsize, pic[0]->width, pic[0]->height);

    free(pic);
    result->push(dst, &gTrBmpType);
    return result;
}


GPContents* TrPackageOutput(GPContents* inputs)
{
    GPContents* result = new GPContents;
    const char* file = "output_fix.jpg";
    TrWritePixels((TrBmp*)inputs->get(0), file);
    return result;
}
GPContents* TrPackageFitCompute(GPContents* inputs)
{
    GPContents* result = new GPContents;
    static TrBmp* target = NULL;
    if (!target)
    {
        target = TrLoadPixels("output.jpg");
    }
    TrBmp* output = (TrBmp*)inputs->get(0);
    double* fit = new double;
    *fit = -TrCompareBmp(output, target);
    result->push(fit, &gTrDoubleType);
    return result;
}
GPContents* TrPackageInput(GPContents* inputs)
{
    /*FIXME*/
    GPContents* result = new GPContents;
    const char* file = "input.jpg";
    static TrBmp* gDst = NULL;
    if (!gDst)
    {
        gDst = TrLoadPixels(file);
    }
    result->push(gDst, NULL);
    return result;
}
GPContents* TrPackageInputTarget(GPContents* inputs)
{
    /*FIXME*/
    GPContents* result = new GPContents;
    const char* file = "output.jpg";
    static TrBmp* gDst = NULL;
    if (!gDst)
    {
        gDst = TrLoadPixels(file);
    }
    result->push(gDst, NULL);
    return result;
}

GPContents* TrPackageFilterMatrixRegress(GPContents* inputs)
{
    GPContents* result = new GPContents;
    assert(inputs->size()==3);
    TrBmp* src = (TrBmp*)inputs->get(0);
    TrBmp* dst = (TrBmp*)inputs->get(1);
    TrRegreeMode* mode = (TrRegreeMode*)inputs->get(2);
    TrFilterMatrix* m = TrRegressMatrix(src, dst, mode);
    result->push(m, &gTrFilterMatrixType);
    return result;
}
GPContents* TrPackageFilterTransformFromRegress(GPContents* inputs)
{
    return TrPackageFilterMatrix(inputs);
}
