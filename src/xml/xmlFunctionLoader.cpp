#include "xml/xmlFunctionLoader.h"
#include <vector>
#include <list>
#include <iostream>

using namespace std;

vector<vector<int> > xmlFunctionLoader::getCombo(int funcId)
{
    vector<vector<int> > result;
    if (!validId(funcId))
    {
        return result;
    }
    result = mFunctions[funcId].inputs;
    return result;
}

vector<int> xmlFunctionLoader::getType(int funcId)
{
    vector<int> result;
    if (!validId(funcId))
    {
        return result;
    }
    result = mFunctions[funcId].statusType;
    return result;
}

void xmlFunctionLoader::attributeUnflatten()
{
    if (NULL!=mAttributes)
    {
        vector<xmlReader::package*> funcs = loadMain(mAttributes);
        for (int i=0; i<funcs.size(); ++i)
        {
            loadFunc(funcs[i]);
            loadStatus(funcs[i]);
        }
    }
}


vector<xmlReader::package*> xmlFunctionLoader::loadMain(package* p)
{
    libName = p->name;
    vector<xmlReader::package*> result;
    const vector<xmlReader::package*> children = p->children;
    //Load Function Table and Statuc Table
    for (int i=0; i<children.size(); ++i)
    {
        xmlReader::package* cur = children[i];
        if (cur->name == "functionTable" )
        {
            for (int j=0; j<cur->attr.size(); ++j)
            {
                xmlFunctionLoader::function func;
                func.name = cur->attr[j];
                mFunctions.push_back(func);
            }
        }
        else if(cur->name == "statusTable")
        {
            for (int j=0; j<cur->attr.size(); ++j)
            {
                status s;
                s.name = cur->attr[j];
                mStatus.push_back(s);
            }
        }
    }
    //Load fit and output, collent function
    for(int i=0; i<children.size(); ++i)
    {
        xmlReader::package* cur = children[i];
        //TODO Support multi output and fit
        if (cur->name == "fit")
        {
            mFit = findFunction(cur->attr[0]);
        }

        else if (cur->name == "output")
        {
            mOutput = findFunction(cur->attr[0]);
        }
        else if (cur->name == "input")
        {
            for (int j=0; j<cur->attr.size(); ++j)
            {
                mInputs.push_back(findFunction(cur->attr[j]));
            }
        }
        else
        {
            result.push_back(cur);
        }
    }
    return result;
}
int xmlFunctionLoader::findFunction(string name)
{
    int result = -1;
    for (int i=0; i<mFunctions.size(); ++i)
    {
        if (mFunctions[i].name == name)
        {
            result = i;
            break;
        }
    }
    return result;
}
int xmlFunctionLoader::findStatus(string name)
{
    int result = -1;
    for (int i=0; i<mStatus.size(); ++i)
    {
        if (mStatus[i].name == name)
        {
            result = i;
            break;
        }
    }
    return result;
}
void xmlFunctionLoader::loadFunc(package* p)
{
    int id = findFunction(p->name);
    if (id == -1) return; //Not found
    xmlFunctionLoader::function& func = mFunctions[id];
    //To avoid repeat func define
    func.inputs.clear();
    func.statusType.clear();
    //Load attr
    for (int i=0; i<p->children.size(); ++i)
    {
        xmlReader::package* cur = p->children[i];
        if (cur->name == "inputs")
        {
            vector<int> input;
            for (int j=0; j<cur->attr.size(); ++j)
            {
                int inp = findFunction(cur->attr[j]);
                if (-1==inp)
                {
                    input.clear();
                    break;
                }
                input.push_back(inp);
            }
            if (!input.empty())
            {
                func.inputs.push_back(input);
            }
        }
        else if(cur->name == "status")
        {
            for (int j=0; j<cur->attr.size(); ++j)
            {
                int sta = findStatus(cur->attr[j]);
                if (sta != -1)
                {
                    func.statusType.push_back(sta);
                }
            }
        }
    }
}
void xmlFunctionLoader::loadStatus(xmlReader::package* p)
{
    int id = findStatus(p->name);
}

void xmlFunctionLoader::subClear()
{
    mFunctions.clear();
    mStatus.clear();
    mOutput = 0;
    mFit = 0;
    mInputs.clear();
}

void xmlFunctionLoader::printFunc(xmlFunctionLoader::function& func)
{
    for (int j=0; j<func.inputs.size(); ++j)
    {
        cout <<"The "<<j << " combo:  ";
        vector<int>& input = func.inputs[j];
        for (int k=0; k<input.size(); ++k)
        {
            cout <<" "<<input[k];
        }
        cout <<endl;
    }
    cout <<"status = ";
    for (int j=0; j <func.statusType.size(); ++j)
    {
        cout <<" "<<func.statusType[j];
    }
    cout << endl;
}

void xmlFunctionLoader::printStatus(status& sta)
{
    cout << "Alloc: " << sta._alloc << endl;
    cout << "Free: " << sta._free << endl;
    cout << "Print: " << sta._print << endl;
    cout << "Load: " << sta._load << endl;
    cout << "Copy: " << sta._copy << endl;
    cout << "Vary: " << sta._vary << endl;
}

void xmlFunctionLoader::print()
{
    cout << "Inputs = ";
    for (int i=0; i < mInputs.size(); ++i)
    {
        cout << mInputs[i]<<" ";
    }
    cout <<endl;
    cout << "output = "<<mOutput<<"    fit = "<<mFit<<endl<<endl;
    cout <<" Function Amount = "<<mFunctions.size()<<endl;
    for (int i=0; i<mFunctions.size(); ++i)
    {
        xmlFunctionLoader::function& func = mFunctions[i];
        cout << "name = "<<func.name <<"    id = "<<i<<endl;
        printFunc(func);
    }
    cout << " Status Types Amount = "<<mStatus.size()<<endl;
    for (int i=0; i<mStatus.size(); ++i)
    {
        xmlFunctionLoader::status& sta = mStatus[i];
        cout << "name = "<<sta.name <<"    id = "<<i<<endl;
        printStatus(sta);
    }
}
