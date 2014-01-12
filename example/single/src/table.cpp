#include "user/IFunctionTable.h"
#include "package.h"
#include "status_user.h"

using namespace std;

const char* Name[]=
{
"TrPackageFitCompute",
"TrPackageOutput",
"TrPackageInput",
"TrPackageInputTarget",
"TrPackageFilterMatrix",
"TrPackageFilterMatrixRegress",
"TrPackageFilterTransformFromRegress",
"TrPackageCompse",
"TrPackageCompseFromRegress",
"TrPackageCompseRegress",
"TrPackageSaturation",
"TrFilterMatrix_alloc",
"TrFilterMatrix_free",
"TrFilterMatrix_vary",
"TrFilterMatrix_copy",
"TrFilterMatrix_print",
"TrFilterMatrix_load",
"float_alloc",
"float_free",
"float_vary",
"float_copy",
"float_print",
"float_load",
"TrRegreeMode_alloc",
"TrRegreeMode_free",
"TrRegreeMode_vary",
"TrRegreeMode_copy",
"TrRegreeMode_print",
"TrRegreeMode_load"
};

void* f[]=
{
(void*)TrPackageFitCompute,
(void*)TrPackageOutput,
(void*)TrPackageInput,
(void*)TrPackageInputTarget,
(void*)TrPackageFilterMatrix,
(void*)TrPackageFilterMatrixRegress,
(void*)TrPackageFilterTransformFromRegress,
(void*)TrPackageCompse,
NULL,
NULL,
(void*)TrPackageSaturation,
(void*)TrFilterMatrix_alloc,
(void*)TrFilterMatrix_free,
(void*)TrFilterMatrix_vary,
(void*)TrFilterMatrix_copy,
(void*)TrFilterMatrix_print,
(void*)TrFilterMatrix_load,
(void*)float_alloc,
(void*)float_free,
(void*)float_vary,
(void*)float_copy,
(void*)float_print,
(void*)float_load,
(void*)TrRegreeMode_alloc,
(void*)TrRegreeMode_free,
(void*)TrRegreeMode_vary,
(void*)TrRegreeMode_copy,
(void*)TrRegreeMode_print,
(void*)TrRegreeMode_load
};



int gNumber = sizeof(f)/sizeof(void*);
