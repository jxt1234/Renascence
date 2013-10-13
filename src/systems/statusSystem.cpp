#include "system/statusSystem.h"
#include <fstream>
#include <iostream>
#include "utils/debug.h"
void statusSystem::loadFuncXml(xmlFunctionLoader& loader)
{
    vector<int> factorType = initStatusType();
    mStatusType.clear();
    for (int i=0; i< loader.size(); ++i)
    {
        vector<int> status = loader.getType(i);
        for (int i=0; i<status.size(); ++i)
        {
            status[i] = factorType[status[i]];
        }
        mStatusType.push_back(status);
    }
}

void statusSystem::loadTable(const char* file)
{
    vector<int> factorType=initStatusType();
    ifstream readFile;
    readFile.open(file);
    if(readFile.fail())
    {
        return;
    }
    mStatusType.clear();
    for (int i=0; i<gAllFunctionsNumber; ++i)
    {
        int num;
        readFile>>num;
        vector<int> type;
        for (int j=0; j<num; ++j)
        {
            int temp;
            readFile>>temp;
            type.push_back(factorType[temp]);
        }
        mStatusType.push_back(type);
    }
    for (int i=0; i<gAllFunctionsNumber; ++i)
    {
        const vector<int>& type = mStatusType[i];
    }
}

int statusSystem::allocateStatus(int functionId)
{
    if (functionId >= gAllFunctionsNumber)
    {
        FUNC_PRINT(functionId);
        return -1;
    }
    const vector<int>& type = mStatusType[functionId];
    return (status_allocSet(type));
}

//Currently I don't support repeat = false, just set every function for a statusId
vector<int> statusSystem::allocateStatus(const vector<int>& functionIds, bool repeat)
{
    vector<int> result;
    for (int i=0; i<functionIds.size(); ++i)
    {
        const vector<int>& type = mStatusType[i];
        result.push_back(status_allocSet(type));
    }
    return result;
}

//Just An interface
vector<void*> basicStatusMap(int statusId)
{
    return status_queryContent(statusId);
}

statusSystem::~statusSystem()
{
    status_clear();
    destroyStatus();
}
