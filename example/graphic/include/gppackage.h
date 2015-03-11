#include "user/package.h"
extern"C"
{
    /*GP BEGIN*/
    GPContents* TrPackageFitCompute(GPContents*);
    GPContents* TrPackageOutput(GPContents*);
    GPContents* TrPackageInput(GPContents*);
    GPContents* TrPackageInputTarget(GPContents*);
    /*TrFilterMatrix is status*/
    GPContents* TrPackageFilterMatrix(GPContents*);
    /*TrFilterMatrix is inputs[1]*/
    /*Input TrBmp src, TrBmp dst, status: TrRegressMode, Output: TrFilterMatrix*/
    GPContents* TrPackageFilterMatrixRegress(GPContents*);
    /*Input: TrBmp TrFilterMatrix, Output TrBmp*/
    GPContents* TrPackageFilterTransformFromRegress(GPContents*);
    GPContents* TrPackageCompse(GPContents*);
    /*Saturation*/
    GPContents* TrPackageOutput(GPContents*);
    GPContents* TrPackageInput(GPContents*);
    GPContents* TrPackageInputTarget(GPContents*);
    /*TrFilterMatrix is status*/
    GPContents* TrPackageFilterMatrix(GPContents*);
    /*TrFilterMatrix is inputs[1]*/
    /*Input TrBmp src, TrBmp dst, status: TrRegressMode, Output: TrFilterMatrix*/
    GPContents* TrPackageFilterMatrixRegress(GPContents*);
    /*Input: TrBmp TrFilterMatrix, Output TrBmp*/
    GPContents* TrPackageFilterTransformFromRegress(GPContents*);
    GPContents* TrPackageCompse(GPContents*);
    /*Saturation*/
    GPContents* TrPackageSaturation(GPContents*);
}
