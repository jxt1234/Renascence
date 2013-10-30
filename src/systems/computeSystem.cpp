#include <iostream>
#include <fstream>
#include "system/computeSystem.h"
#include "utils/debug.h"
#include "math/carryArray.h"
#include "system/system_lib.h"
#include <assert.h>

#define FUNCTION_TABLE_FILE_DEFAULT "db/function.table"

computeSystem::computeSystem()
{
}
const std::vector<std::vector<int> >& computeSystem::getAvailableFunctionInputs(int functionId)
{
    static vector<vector<int> > nullVector;
    if (functionId >=0 && functionId < mBasicTable.size())
    {
        return mBasicTable[functionId];
    }
    return nullVector;
}

computeSystem::~computeSystem()
{
}

vector<int> computeSystem::getAllFunctionIDs()
{
    vector<int> result;
    for (int i=0; i<mFunctions.size(); ++i)
    {
        result.push_back(i);
    }
    return result;
}

void computeSystem::clear()
{
    mBasicTable.clear();
    mStatusTable.clear();
    mInputId.clear();
    mOutputId.clear();
    mFunctions.clear();
}

vector<int> computeSystem::loadStatus(const vector<xmlFunctionLoader::status>& sta, void* handle)
{
    vector<int> typeId;
    for (int i=0; i<sta.size(); ++i)
    {
#define FINDFUNC(type, subname) type subname = (type)system_find_func(handle, (sta[i].name+#subname).c_str());
        FINDFUNC(statusAllocMethod, _alloc);
        FINDFUNC(statusVaryMethod, _free);
        FINDFUNC(statusVaryMethod, _vary);
        FINDFUNC(statusCopyMethod, _copy);
        FINDFUNC(statusPrintMethod, _print);
        FINDFUNC(statusLoadMethod, _load);
#undef FINDFUNC
        int id = status_allocType(0, sta[i].name, _alloc, _free, _vary, _copy, _print, _load);
        typeId.push_back(id);
    }
    return typeId;
}

void computeSystem::loadFuncXml(xmlFunctionLoader& loader, void* &handle)
{
    if (NULL == handle)
    {
        handle = system_load_lib(loader.libName.c_str());
    }
    assert(NULL!=handle);
    vector<int> statusTypeId = loadStatus(loader.getStatus(), handle);
    int offset = mFunctions.size();
    mOutputId.push_back(loader.mFit);
    const vector<xmlFunctionLoader::function>& functions = loader.getFunc();
    for (int i=0; i<functions.size(); ++i)
    {
        const xmlFunctionLoader::function& f = functions[i];
        /*Load function handle*/
        computeFunction func = (computeFunction)system_find_func(handle, f.name.c_str());
        assert(NULL!=func);
        mFunctions.push_back(func);
        /*Load function status*/
        vector<int> statusType = f.statusType;
        vector<int> newST;
        for (int j=0; j<statusType.size(); ++j)
        {
            int cur = statusType[j];
            newST.push_back(statusTypeId[cur]);
        }
        mStatusTable.push_back(newST);
        /*Load function inputs*/
        vector<vector<int> > newCombo = f.inputs;
        for (int j=0; j<newCombo.size(); ++j)
        {
            for (int k=0; k<newCombo[j].size(); ++k)
            {
                newCombo[j][k]+=offset;
            }
        }
        mBasicTable.push_back(newCombo);
    }
    mInputId = loader.mInputs;
}


int computeSystem::allocateStatus(int id)
{
    if (id < 0) return -1;
    return status_allocSet(mStatusTable[id]);
}

void computeSystem::print()
{
    cout << "All functions number is "<<mFunctions.size()<<endl;
    cout<<"Input function Id"<<endl;
    for (int i=0; i< mInputId.size(); ++i)
    {
        cout<<mInputId[i]<<"    ";
    }
    cout << endl;
    cout << "Output funciton Id"<<endl;
    for (int i=0; i< mOutputId.size(); ++i)
    {
        cout << mOutputId[i]<<"    ";
    }
    cout << endl;
    cout << "Status table is "<<endl;
    for (int i=0; i<mStatusTable.size(); ++i)
    {
        cout << "The "<<i<<" functions' status:"<<endl;
        for (int j=0; j<mStatusTable[i].size(); ++j)
        {
            cout << mStatusTable[i][j]<<" ";
        }
        cout <<endl;
    }
    cout << "Basic table is "<<endl;
    for (int i=0; i<mBasicTable.size(); ++i)
    {
        cout <<endl<<"The "<<i<<" function "<<endl;
        for(int j=0; j<mBasicTable[i].size(); ++j)
        {
            cout <<endl<<" combo "<<j<<": ";
            for(int k=0; k<mBasicTable[i][j].size(); ++k)
            {
                cout << mBasicTable[i][j][k]<<" ";
            }
        }
    }
}
