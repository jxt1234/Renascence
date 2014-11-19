#include "package.h"
#include <assert.h>
#define RETURN_GP \
    GP_Output out;\
    out.output.push_back(result);\
    return out;

GP_Output TrPackageFilterMatrix(vector<void*> inputs)
{
    assert(inputs.size() == 2);
    GP_Output::GP_Unit result;
    TrBmp* dst = TrFilterMatrixTransform((TrBmp*)inputs[0], (TrFilterMatrix*)inputs[1]);
    result.content = dst;
    result.freeCallBack = (GP_FreeFunction)TrFreeBmp;
    RETURN_GP;
}

GP_Output TrPackageSaturation(vector<void*> inputs)
{
    assert(inputs.size() == 2);
    GP_Output::GP_Unit result;
    TrBmp* dst = TrSaturation((TrBmp*)inputs[0], 2*((float*)inputs[1])[0]);
    result.content = dst;
    result.freeCallBack = (GP_FreeFunction)TrFreeBmp;
    RETURN_GP;
}

//以第一幅图像的大小为最终的图像大小
GP_Output TrPackageCompse(vector<void*> inputs)
{
    assert(inputs.size() >= 2);
    GP_Output::GP_Unit result;
    int picsize = inputs.size() - 1;
    TrBmp** pic = (TrBmp**)(malloc(picsize*sizeof(TrBmp)));
    float* factor = (float*)inputs[picsize];
    for (int i=0; i<picsize; ++i)
    {
        pic[i] = (TrBmp*)inputs[i];
    }
    TrBmp* dst = TrMixPicture(pic, factor, picsize, pic[0]->width, pic[0]->height);

    free(pic);
    result.content = dst;
    result.freeCallBack = (GP_FreeFunction)TrFreeBmp;
    RETURN_GP;
}


GP_Output TrPackageOutput(vector<void*> inputs)
{
    assert(!inputs.empty());
    const char* file = "output_fix.jpg";
    TrWritePixels((TrBmp*)inputs[0], file);
    GP_Output res;
    return res;
}
static void doubleDelete(double* p)
{
    delete p;
}
GP_Output TrPackageFitCompute(vector<void*> inputs)
{
    assert(!inputs.empty());
    GP_Output::GP_Unit result;
    static TrBmp* target = NULL;
    if (!target)
    {
        target = TrLoadPixels("output.jpg");
    }
    TrBmp* output = (TrBmp*)inputs[0];
    double* fit = new double;
    *fit = -TrCompareBmp(output, target);
    result.content = (void*)fit;
    result.freeCallBack = (GP_FreeFunction)doubleDelete;
    RETURN_GP;
}
GP_Output TrPackageInput(vector<void*> inputs)
{
    GP_Output::GP_Unit result;
    const char* file = "input.jpg";
    static TrBmp* gDst = NULL;
    if (!gDst)
    {
        gDst = TrLoadPixels(file);
    }
    result.content = (void*)gDst;
    //Don't free this
    result.freeCallBack = NULL;
    RETURN_GP;
}
GP_Output TrPackageInputTarget(vector<void*> inputs)
{
    GP_Output::GP_Unit result;
    const char* file = "output.jpg";
    static TrBmp* gDst = NULL;
    if (!gDst)
    {
        gDst = TrLoadPixels(file);
    }
    result.content = (void*)gDst;
    //Don't free this
    result.freeCallBack = NULL;
    RETURN_GP;
}

GP_Output TrPackageFilterMatrixRegress(vector<void*> inputs)
{
    GP_Output::GP_Unit result;
    assert(inputs.size()==3);
    TrBmp* src = (TrBmp*)inputs[0];
    TrBmp* dst = (TrBmp*)inputs[1];
    TrRegreeMode* mode = (TrRegreeMode*)inputs[2];
    TrFilterMatrix* m = TrRegressMatrix(src, dst, mode);
    result.content = (void*)m;
    result.freeCallBack = (GP_FreeFunction)TrFilterMatrixFree;
    RETURN_GP;
}
GP_Output TrPackageFilterTransformFromRegress(vector<void*> inputs)
{
    return TrPackageFilterMatrix(inputs);
}
