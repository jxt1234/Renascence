#include "system/xmlDataBase.h"
#include <assert.h>
using namespace std;
void xmlDataBase::vQueryStatus(int id, std::string& name, std::string& libName)
{
    assert(id < mStatus.size() && id >=0);
    name = mStatus[id].first;
    int libid = mStatus[id].second;
    libName = mLibName[libid];
}
void xmlDataBase::vQueryFunction(int id, std::string& name, std::string& libName)
{
    assert(id < mFunctions.size() && id >=0);
    name = mFunctions[id].first;
    int libid = mFunctions[id].second;
    libName = mLibName[libid];
}

xmlDataBase::xmlDataBase()
{
}

xmlDataBase::~xmlDataBase()
{
}

void xmlDataBase::loadXml(xmlFunctionLoader& loader)
{
    const vector<xmlFunctionLoader::function>& funcs = loader.getFunc();
    int libId = mLibName.size();
    mLibName.push_back(loader.libName);
    for (int i=0; i<funcs.size(); ++i)
    {
        pair<string, int> p(funcs[i].name, libId);
        mFunctions.push_back(p);
    }
    const vector<xmlFunctionLoader::status>& stat = loader.getStatus();
    for (int i=0; i<stat.size(); ++i)
    {
        pair<string, int> p(stat[i].name, libId);
        mStatus.push_back(p);
    }
}
