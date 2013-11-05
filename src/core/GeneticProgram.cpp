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
#include "core/GeneticProgram.h"
#include "core/GP_XmlString.h"
#include "utils/debug.h"
#include <assert.h>
#include <list>
using namespace std;

static void GP_OutputFree(GP_Output* save)
{
    assert(NULL!=save);
    for (int i=0; i<save->output.size(); ++i)
    {
        GP_Output::GP_Unit& unit = save->output[i];
        if (NULL!=unit.freeCallBack)
        {
            unit.freeCallBack(unit.content);
        }
    }
    delete save;
}

static void GP_Output_SetNoFree(GP_Output& result)
{
    for (int i=0; i<result.output.size(); ++i)
    {
        result.output[i].freeCallBack = NULL;
    }
}

vector<int> GeneticProgram::getStatusId()
{
    vector<int> queue = getPointQueue(mRoot);
    vector<int> status;
    for (int i=0; i<queue.size()/3; ++i)
    {
        status.push_back(queue[3*i+1]);
    }
    return status;
}

void GeneticProgram::replaceFunctionId(vector<pair<int, int> > pairList)
{
    vector<GeneticPoint*> points = searchAllPoints();
    for (int i=0; i<points.size(); ++i)
    {
        for (int j=0; j<pairList.size(); ++j)
        {
            if (points[i]->functionId == pairList[j].first)
            {
                points[i]->functionId = pairList[j].second;
                break;
            }
        }
    }
}

vector<int> GeneticProgram::getFuncId()
{
    vector<int> queue = getPointQueue(mRoot);
    vector<int> funcs;
    for (int i=0; i<queue.size()/3; ++i)
    {
        funcs.push_back(queue[3*i+0]);
    }
    return funcs;
}

static bool loadNumbers(const vector<int> &numbers, int &cur, GeneticPoint* point,int &pointNumber)
{
    if (numbers.size() < cur + 2)
    {
        return false;
    }
    point->functionId = numbers[cur];
    point->statusId = numbers[cur+1];
    pointNumber = numbers[cur+2];
    cur+=3;
    return true;
}

static void saveNumbers(vector<int>& outputNumbers, GeneticPoint* point)
{
    outputNumbers.push_back(point->functionId);
    outputNumbers.push_back(point->statusId);
    outputNumbers.push_back(point->children.size());
}

GeneticProgram::GeneticProgram()
{
    mRoot = NULL;
}

GeneticProgram::GeneticProgram(const GeneticProgram& program)
{
    mRoot = NULL;
    copyPoints(mRoot, program.mRoot);
}

GeneticProgram::GeneticProgram(const vector<int> &numbers)
{
    mRoot = NULL;
    replacePoint(numbers, mRoot);
}
GeneticProgram::GeneticProgram(const char* file)
{
//To be completed 
    mRoot = NULL;
}
void GeneticProgram::operator=(const GeneticProgram& program)
{
    copyPoints(mRoot, program.mRoot);
}


vector<int> GeneticProgram::getPointQueue(GeneticPoint* point)
{
    vector<int> result;
    list<GeneticPoint*> cacheQueue;
    if (NULL == point)
    {
        return result;
    }
    cacheQueue.push_back(point);
    while(!cacheQueue.empty())
    {
        GeneticPoint* current = *(cacheQueue.begin());
        saveNumbers(result, current);
        for (int i=0; i<current->children.size(); ++i)
        {
            cacheQueue.push_back(current->children[i]);
        }
        cacheQueue.erase(cacheQueue.begin());
    }
    return result;
}
void GeneticProgram::copyPoints(GeneticPoint* self, GeneticPoint* source)
{
    if (NULL==source || source == self)
    {
        return;
    }
    //Start to copy, It's permitted to copy self(source is in this tree)
    vector<int> replaceQueue = getPointQueue(source);
    //Copy alloc all status 
    for (int i=0; i<replaceQueue.size()/3; ++i)
    {
        replaceQueue[i*3+1] = status_CopyAllocSet(replaceQueue[i*3+1]);
    }
    replacePoint(replaceQueue, self);
}

void GeneticProgram::clear()
{
    if (mRoot!=NULL)
    {
        deleteUnit(mRoot);
        delete mRoot;
        mRoot = NULL;
    }
}

GP_Output GeneticProgram::compute(IFunctionDataBase* table)
{
    GP_Output output = computeUnit(table, mRoot);
    return output;
}

void GeneticProgram::deleteUnit(GeneticPoint* point)
{
    if (NULL!=point)
    {
        for (int i; i < point->children.size(); ++i)
        {
            deleteUnit(point->children[i]);
            delete point->children[i];
        }
        point->children.clear();
        status_freeSet(point->statusId);
        point->statusId = -1;
    }
}


bool GeneticProgram::replacePoint(const vector<int> &numbers, int n)
{
    vector<GeneticPoint*> queue = searchAllPoints();
    GeneticPoint* point = queue[n%queue.size()];
    return replacePoint(numbers, point);
}
bool GeneticProgram::replacePoint(const vector<int> &numbers, GeneticPoint* point)
{
    vector<GeneticPoint*> cacheQueue;
    int cur = 0;
    /*If point==NULL and its a point in this Tree, then the mRoot must be NULL*/
    if (NULL==point)
    {
        point = new GeneticPoint;
        mRoot = point;
    }
    else
    {
        deleteUnit(point);
    }
    cacheQueue.push_back(point);
    while(!cacheQueue.empty())
    {
        GeneticPoint* current = cacheQueue[0];
        int pointNumber;
        if (loadNumbers(numbers, cur, current, pointNumber))
        {
            for (int i=0; i < pointNumber; ++i)
            {
                GeneticPoint* input = new GeneticPoint;
                current->children.push_back(input);
                cacheQueue.push_back(input);
            }
            cacheQueue.erase(cacheQueue.begin());
        }
        else
        {
            clear();
            return false;
        }
    }
    return true;
}

vector<GeneticPoint*> GeneticProgram::searchAllPoints()
{
    vector<GeneticPoint*> pointQueue;
    vector<GeneticPoint*> result;
    pointQueue.push_back(mRoot);
    result.push_back(mRoot);
    while(!pointQueue.empty())
    {
        GeneticPoint* current = pointQueue[0];
        if (NULL==current)
        {
            return result;
        }
        for (int i=0; i<current->children.size(); ++i)
        {
            pointQueue.push_back(current->children[i]);
            result.push_back(current->children[i]);
        }
        pointQueue.erase(pointQueue.begin());
    }
    return result;
}

GP_Output GeneticProgram::computeUnit(IFunctionDataBase* table, GeneticPoint* point)
{
    assert(NULL!=point);
    GP_Output result;
    //Early out
    if (NULL!=point->save)
    {
        result = *(point->save);
        //Clear freeCallBack function to avoid free
        GP_Output_SetNoFree(result);
        return result;
    }
    computeFunction comp = table->vGetCompute(point->functionId);
    vector<GP_Output::GP_Unit> inputMap;
    vector<void*> children;
    //Get Inputs from childern point
    for (int i=0; i<point->children.size(); ++i)
    {
        GP_Output out = computeUnit(table, point->children[i]);
        vector<GP_Output::GP_Unit>& output_unit = out.output;
        for (int j=0; j<output_unit.size(); ++j)
        {
            inputMap.push_back(output_unit[j]);
            children.push_back(output_unit[j].content);
        }
    }
    vector<void*> constValue;
    if (-1 != point->statusId)
    {
        constValue = status_queryContent(point->statusId);
    }
    {
#ifdef DEBUG_TIMES
        GP_Clock c(point->functionId);
#endif
        result = comp(children, constValue);
    }
    //Free All children' memory
    for (int i=0; i < inputMap.size(); ++i)
    {
        GP_Output::GP_Unit& out = inputMap[i];
        if (NULL!=out.freeCallBack)
        {
            out.freeCallBack(out.content);
        }
    }
    return result;
}

GeneticProgram::~GeneticProgram()
{
    clear();
}
void GeneticProgram::loadUnitFunction(vector<int>& result, int functionId, int statusId, int number)
{
    result.push_back(functionId);
    result.push_back(statusId);
    result.push_back(number);
}


string GeneticProgram::xmlPrint(IDataBase* data)
{
    assert(NULL!=data);
    assert(NULL!=mRoot);
    ostringstream res;
    xmlPrintUnit(res, data, mRoot);
    return res.str();
}
void GeneticProgram::xmlPrintUnit(ostringstream& res, IDataBase* data, GeneticPoint* point)
{
    string funcName, libName;
    res << "<"<< GP_XmlString::node<<">"<<endl;
    /*If the point is saved, just print the value*/
    if (NULL!=point->save)
    {
        res << "<result>"<<endl;
        vector<void*> output;
        GP_Output_collect(output, *(point->save));
        vector<int> outputType;
        data->vQueryOutput(point->functionId, outputType);
        status_printSetWithType(output, outputType, res);
        res << "</result>"<<endl;
    }
    data->vQueryFunction(point->functionId, funcName, libName);
    res<<"<"<<GP_XmlString::lib<<">"<<libName<<"</"<<GP_XmlString::lib<<">\n";
    res<<"<"<<GP_XmlString::func<<">"<<funcName<<"</"<<GP_XmlString::func<<">\n";
    if (point->statusId >= 0)
    {
        res<<"<"<<GP_XmlString::status<<">\n";
        res<<status_printSet(point->statusId);
        res<<"</"<<GP_XmlString::status<<">\n";
    }
    res <<"<"<< GP_XmlString::children<<">"<<endl;
    for (int i=0; i<point->children.size(); ++i)
    {
        xmlPrintUnit(res, data, point->children[i]);
    }
    res <<"</"<< GP_XmlString::children<<">"<<endl;
    res << "</"<< GP_XmlString::node<<">"<<endl;
}

void GeneticProgram::save(IFunctionDataBase* table, const std::vector<int>& functionIds)
{
    assert(NULL!=mRoot);
    reset();
    computeUnitSave(table, mRoot, functionIds);
}

void GeneticProgram::reset()
{
    vector<GeneticPoint*> queue = searchAllPoints();
    for (int i=0; i<queue.size(); ++i)
    {
        GeneticPoint* point = queue[i];
        if (NULL!=point->save)
        {
            GP_OutputFree(point->save);
            point->save = NULL;
        }
    }
}

void GeneticProgram::computeUnitSave(IFunctionDataBase* table, GeneticPoint* point, const std::vector<int> functionIds)
{
    for (int i=0; i<functionIds.size(); ++i)
    {
        if (point->functionId == functionIds[i])
        {
            GP_Output out;
            out = computeUnit(table, point);
            assert(NULL==point->save);
            point->save = new GP_Output;
            *(point->save) = out;
            return;
        }
    }
    for (int i=0; i<point->children.size(); ++i)
    {
        computeUnitSave(table, point->children[i], functionIds);
    }
}
