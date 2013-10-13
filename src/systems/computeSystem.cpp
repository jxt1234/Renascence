#include <iostream>
#include <fstream>
#include "system/computeSystem.h"
#include "utils/debug.h"
#include "math/carryArray.h"
#define FUNCTION_TABLE_FILE_DEFAULT "db/function.table"

static computeFunction myMapFunction(int functionId)
{
    int id = functionId%gAllFunctionsNumber;
    return gAllFunctions[id];
}

computeSystem::computeSystem()
{
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

mapFunction computeSystem::getMapFunction()
{
    mapFunction result;
    result = myMapFunction;
    return result;
}
computeFunction computeSystem::map(int functionId)
{
    int id = functionId%mFunctions.size();
    return mFunctions[id];
}

const vector<int>& computeSystem::getAvailablemFunctions(int functionId)
{
    int id = functionId%mTable.size();
    return mTable[id];
}

void computeSystem::clear()
{
    mBasicTable.clear();
    mTable.clear();
    mInputId.clear();
    mOutputId.clear();
    mFunctions.clear();
    initFunctions();
}

void computeSystem::loadFuncXml(xmlFunctionLoader& loader)
{
    clear();
    mOutputId.push_back(loader.mFit);
    for (int i=0; i<loader.size(); ++i)
    {
        mBasicTable.push_back(loader.getCombo(i));
    }
    mInputId = loader.mInputs;
}

void computeSystem::loadTable(const char* file)
{
    clear();
    ifstream readFile;
    if (NULL==file)
    {
        readFile.open(FUNCTION_TABLE_FILE_DEFAULT);
    }
    else
    {
        readFile.open(file);
    }
    if (!readFile.fail())
    {
        for (int i=0; i < gAllFunctionsNumber; ++i)
        {
            mFunctions.push_back(gAllFunctions[i]);
        }
        //Input numbers
        int inputsNumber;
        readFile >> inputsNumber;
        for (int i=0; i<inputsNumber; ++i)
        {
            int temp;
            readFile>>temp;
            mInputId.push_back(temp);
        }
        //Output numbers
        int outputsNumber;
        readFile>>outputsNumber;
        for (int i=0; i < outputsNumber; ++i)
        {
            int temp;
            readFile >> temp;
            mOutputId.push_back(temp);
        }
        //Table
        for (int i=0; i < mFunctions.size(); ++i)
        {
            int inputsNumber;
            readFile >> inputsNumber;
            vector<int> tempVector;
            for (int j=0; j<inputsNumber;++j)
            {
                int temp;
                readFile>>temp;
                tempVector.push_back(temp);
            }
            mTable.push_back(tempVector);
        }
        //Every functions' min and max inputs number
        mInputNumbers.clear();
        for (int i=0; i<mFunctions.size(); ++i)
        {
            int min, max;
            readFile>>min>>max;
            pair<int, int> temp(min, max);
            mInputNumbers.push_back(temp);
        }
        readFile.close();
    }
    constructBasicTable();
}
void computeSystem::constructBasicTable()
{
    mBasicTable.clear();
    for (int i=0; i<mFunctions.size(); ++i)
    {
        vector<int> inputFunc = getAvailablemFunctions(i);
        vector<vector<int> > inputs;
        int size = getInputNumbers(i).first;
        carryArray signQueue(size, inputFunc.size());
        while(!signQueue.reachEnd())
        {
            const vector<int>& queue = signQueue.getCurrent();
            vector<int> inputUnit;
            for (int j=0; j<queue.size(); j++)
            {
                inputUnit.push_back(inputFunc[queue[j]]);
            }
            inputs.push_back(inputUnit);
            ++signQueue;
        }
        mBasicTable.push_back(inputs);
    }
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
    cout <<" All functions' inputNumber is "<<endl;
    for (int i=0; i<mInputNumbers.size(); ++i)
    {
        cout << i<<":\t"<<mInputNumbers[i].first <<"\t"<<mInputNumbers[i].second<<endl;
    }
    cout << endl;
    cout << "The match table is "<<endl;
    for (int i=0; i<mTable.size(); ++i)
    {
        vector<int> &temp(mTable[i]);
        cout <<"The inputs of function " << i << "  Can be ";
        for (int j=0; j<temp.size(); ++j)
        {
            cout << temp[j]<<"    ";
        }
        cout << endl;
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
