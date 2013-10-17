#include "core/GeneticProgram.h"
#include "utils/debug.h"
#include <assert.h>
#include <list>
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
    outputNumbers.push_back(point->inputs.size());
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
        for (int i=0; i<current->inputs.size(); ++i)
        {
            cacheQueue.push_back(current->inputs[i]);
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

GP_Output GeneticProgram::compute(const vector<computeFunction>& table)
{
    GP_Output output = computeUnit(table, mRoot);
    return output;
}

void GeneticProgram::deleteUnit(GeneticPoint* point)
{
    if (NULL!=point)
    {
        for (int i; i < point->inputs.size(); ++i)
        {
            deleteUnit(point->inputs[i]);
            delete point->inputs[i];
        }
        point->inputs.clear();
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
                current->inputs.push_back(input);
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
        for (int i=0; i<current->inputs.size(); ++i)
        {
            pointQueue.push_back(current->inputs[i]);
            result.push_back(current->inputs[i]);
        }
        pointQueue.erase(pointQueue.begin());
    }
    return result;
}

GP_Output GeneticProgram::computeUnit(const vector<computeFunction>& table, GeneticPoint* point)
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
    computeFunction compute = table[point->functionId];
    vector<GP_Output::GP_Unit> inputMap;
    vector<void*> inputs;
    //Get Inputs from childern point
    for (int i=0; i<point->inputs.size(); ++i)
    {
        GP_Output out = computeUnit(table, point->inputs[i]);
        vector<GP_Output::GP_Unit>& output_unit = out.output;
        for (int j=0; j<output_unit.size(); ++j)
        {
            inputMap.push_back(output_unit[j]);
            inputs.push_back(output_unit[j].content);
        }
    }
    vector<void*> constValue;
    if (-1 != point->statusId)
    {
        constValue = status_queryContent(point->statusId);
    }
    result = compute(inputs, constValue);
    //Free All inputs' memory
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


string GeneticProgram::xmlPrint(string(*funcPrint)(int))
{
    assert(NULL!=funcPrint);
    assert(NULL!=mRoot);
    ostringstream res;
    xmlPrintUnit(res, funcPrint, mRoot);
    return res.str();
}
void GeneticProgram::xmlPrintUnit(ostringstream& res, string(*funcPrint)(int), GeneticPoint* point)
{
    res<<"<"<<funcPrint(point->functionId)<<">\n";
    if (point->statusId >= 0)
    {
        res<<"<status>\n";
        res<<status_printSet(point->statusId);
        res<<"</status>\n";
    }
    for (int i=0; i<point->inputs.size(); ++i)
    {
        xmlPrintUnit(res, funcPrint, point->inputs[i]);
    }
    res<<"</"<<funcPrint(point->functionId)<<">\n";
}

void GeneticProgram::save(const vector<computeFunction>& table, const std::vector<int>& functionIds)
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

void GeneticProgram::computeUnitSave(const vector<computeFunction>& table, GeneticPoint* point, const std::vector<int> functionIds)
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
    for (int i=0; i<point->inputs.size(); ++i)
    {
        computeUnitSave(table, point->inputs[i], functionIds);
    }
}
