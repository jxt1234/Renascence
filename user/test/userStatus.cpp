#include "status.h"
#include "user/statusUser.h"

vector<int> initStatusType()
{
    vector<int> result;
    result.push_back(status_allocType(sizeof(float)));
    result.push_back(status_allocType(sizeof(int)));
    return result;
}

void destroyStatus()
{
}
