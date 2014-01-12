#include <vector>
#include <stdlib.h>

struct GP_Output
{
    struct GP_Unit
    {
        void* content;
        void(*freeCallBack)(void*);
    };
    std::vector<GP_Unit> output;
};
extern "C"
{

/*It will Allocate memory for the function pointer for the first time*/
}
using namespace std;
extern "C"
{
    GP_Output TrPackageFitCompute(vector<void*> inputs);
    GP_Output TrPackageOutput(vector<void*> inputs);
    GP_Output TrPackageInput(vector<void*> inputs);
    GP_Output TrPackageInputTarget(vector<void*> inputs);
    /*TrFilterMatrix is status*/
    GP_Output TrPackageFilterMatrix(vector<void*> inputs);
    /*TrFilterMatrix is inputs[1]*/
    /*Input TrBmp src, TrBmp dst, status: TrRegressMode, Output: TrFilterMatrix*/
    GP_Output TrPackageFilterMatrixRegress(vector<void*> inputs);
    /*Input: TrBmp TrFilterMatrix, Output TrBmp*/
    GP_Output TrPackageFilterTransformFromRegress(vector<void*> inputs);
    GP_Output TrPackageCompse(vector<void*> inputs);
    GP_Output TrPackageCompseFromRegress(vector<void*> inputs);
    GP_Output TrPackageCompseRegress(vector<void*> inputs);
    /*Saturation*/
    GP_Output TrPackageSaturation(vector<void*> inputs);
}
