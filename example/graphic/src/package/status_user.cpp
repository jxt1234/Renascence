/*AUTO-GENERATE*/
#include "status_user.h"


TrFloatType gFloatType;
TrFilterMatrixType gTrFilterMatrixType;
TrRegreeModeType gTrRegreeModeType;
TrBmpType gTrBmpType;
GPDoubleType gDoubleType;

IStatusType* GP_IStatusType_Create(const std::string& name)
{
    if (name == "float")
    {
        return &gFloatType;
    }
    if (name == "TrFilterMatrix")
    {
        return &gTrFilterMatrixType;
    }
    if (name == "TrRegreeMode")
    {
        return &gTrRegreeModeType;
    }
    if (name == "TrBmp")
    {
        return &gTrBmpType;
    }
    return NULL;
}
