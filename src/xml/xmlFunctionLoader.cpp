/******************************************************************
   Copyright 2013, Jiang Xiao-tang

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
******************************************************************/
#include "xml/xmlFunctionLoader.h"
#include <vector>
#include <list>

using namespace std;

static bool blackName(const string& func)
{
    if ("output" == func || "input" == func || "functionTable" == func || "statusTable" == func || "fit" == func)
    {
        return true;
    }
    return false;
}

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
        if (!blackName(cur->name))
        {
            xmlFunctionLoader::function func;
            func.name = cur->name;
            mFunctions.push_back(func);
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
    if (-1 == result)
    {
        status s;
        s.name = name;
        mStatus.push_back(s);
        result = mStatus.size() - 1;
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
        else if(cur->name == "output")
        {
            for (int j=0; j<cur->attr.size(); ++j)
            {
                int sta = findStatus(cur->attr[j]);
                if (sta != -1)
                {
                    func.outputType.push_back(sta);
                }
            }
        }
        else if (cur->name == "inputType")
        {
            for (int j=0; j<cur->attr.size(); ++j)
            {
                int sta = findStatus(cur->attr[j]);
                if (sta != -1)
                {
                    func.inputType.push_back(sta);
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

void xmlFunctionLoader::printFunc(xmlFunctionLoader::function& func, ostream& output)
{
    for (int j=0; j<func.inputs.size(); ++j)
    {
        output <<"The "<<j << " combo:  ";
        vector<int>& input = func.inputs[j];
        for (int k=0; k<input.size(); ++k)
        {
            output <<" "<<input[k];
        }
        output <<endl;
    }
    output <<"status = ";
    for (int j=0; j <func.statusType.size(); ++j)
    {
        output <<" "<<func.statusType[j];
    }
    output << "output = ";
    for (int j=0; j <func.outputType.size(); ++j)
    {
        output <<" "<<func.outputType[j];
    }
    output << endl;
}

void xmlFunctionLoader::printStatus(status& sta, ostream& output)
{
    output << "Alloc: " << sta._alloc << endl;
    output << "Free: " << sta._free << endl;
    output << "Print: " << sta._print << endl;
    output << "Load: " << sta._load << endl;
    output << "Copy: " << sta._copy << endl;
    output << "Vary: " << sta._vary << endl;
}

void xmlFunctionLoader::print(ostream& output)
{
    output << "Inputs = ";
    for (int i=0; i < mInputs.size(); ++i)
    {
        output << mInputs[i]<<" ";
    }
    output <<endl;
    output << "output = "<<mOutput<<"    fit = "<<mFit<<endl<<endl;
    output <<" Function Amount = "<<mFunctions.size()<<endl;
    for (int i=0; i<mFunctions.size(); ++i)
    {
        xmlFunctionLoader::function& func = mFunctions[i];
        output << "name = "<<func.name <<"    id = "<<i<<endl;
        printFunc(func, output);
    }
    output << " Status Types Amount = "<<mStatus.size()<<endl;
    for (int i=0; i<mStatus.size(); ++i)
    {
        xmlFunctionLoader::status& sta = mStatus[i];
        output << "name = "<<sta.name <<"    id = "<<i<<endl;
        printStatus(sta, output);
    }
}
