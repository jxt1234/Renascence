/******************************************************************
   Copyright 2017, Jiang Xiao-tang

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
#include "GPBasicAdaptor.h"
#include "utils/GPDebug.h"
#include "frontend/GPProducerUtils.h"
#include "frontend/recurse_tree.h"
#include "math/carryGroup.h"
#include <string.h>
#include <sstream>
#include <algorithm>
using namespace std;

static int _loadXn(const string& s)
{
    int n = (int)(s.size());
    int sum = 0;
    for(int i=1; i<n; ++i)
    {
        if (s[i]>='0' && s[i]<='9')
        {
            sum*=10;
            sum += s[i]-'0';
        }
        else
        {
            break;
        }
    }
    return sum;
}


static vector<vector<const GPProducerUtils::func*> > _filterOutputType(const vector<vector<const GPProducerUtils::func*> >& origin, const std::vector<const IStatusType*>& inputType)
{
    vector<vector<const GPProducerUtils::func*> > result;
    for (auto p : origin)
    {
        vector<const IStatusType*> inputs;
        for (auto f : p)
        {
            auto realtypes = f->basic->inputType;
            inputs.insert(inputs.end(), realtypes.begin(), realtypes.end());
        }
        if (inputs.size() == inputType.size())
        {
            result.push_back(p);
        }
    }
    if (result.empty())
    {
        return origin;
    }
    return result;
}

static void _findMatchedFuncton(std::vector<std::vector<const GPProducerUtils::func*> >& warpOutput, const std::vector<const IStatusType*>& outputType, const std::vector<const IStatusType*>& inputType, const GPProducerUtils& mUtils)
{
    carryGroup<const GPProducerUtils::func*> group;
    for(int i=0; i<outputType.size(); ++i)
    {
        group.mBase.push_back(mUtils.getFunctionsForOutput(outputType[i]));
    }
    group.reset();
    do
    {
        auto current = group.current();
        bool valid = true;
        for (auto f : current)
        {
            for (auto t : f->inputs)
            {
                if (find(inputType.begin(), inputType.end(), t) == inputType.end())
                {
                    valid = false;
                    break;
                }
            }
            if (!valid)
            {
                break;
            }
        }
        if (valid)
        {
            warpOutput.push_back(current);
        }
    } while(group.next());
}

/*Check If the real types can contain all inputs from target*/
static bool _ValidateInputs(const vector<const IStatusType*>& real, const vector<const IStatusType*>& target)
{
    if (real.size() < target.size())
    {
        return false;
    }
    /*Generate type counts*/
    std::map<const IStatusType*, int> typescount;
    for (auto t : target)
    {
        if (typescount.find(t)==typescount.end())
        {
            typescount.insert(make_pair(t, 0));
        }
        typescount.find(t)->second++;
    }
    for (auto t : real)
    {
        if (typescount.find(t)==typescount.end())
        {
            GPASSERT(0);//This should be confirm in search
            return false;
        }
        typescount.find(t)->second--;
    }
    for (auto iter : typescount)
    {
        if (iter.second>0)
        {
            return false;
        }
    }
    return true;
}

static std::vector<const IStatusType*> reshape(const std::map<int, const IStatusType*> maps)
{
    std::vector<const IStatusType*> result;
    for (auto iter : maps)
    {
        if (result.size() <= iter.first)
        {
            for (size_t i=result.size(); i<=iter.first; ++i)
            {
                result.push_back(NULL);
            }
        }
        result[iter.first] = iter.second;
    }
    for (auto r : result)
    {
        GPASSERT(NULL!=r);
    }
    return result;
}

static GPFunctionTreePoint* _createOnePoint(const std::vector<const IStatusType*>& outputType, const std::vector<const IStatusType*>& inputType, const GPProducerUtils& mUtils)
{
    vector<vector<const GPProducerUtils::func*> > warpOutput;
    _findMatchedFuncton(warpOutput, outputType, inputType, mUtils);
    warpOutput = _filterOutputType(warpOutput, inputType);
    if (warpOutput.empty())
    {
        return NULL;
    }
    vector<int> avail;
    for (int i=0; i<warpOutput.size(); ++i)
    {
        avail.push_back(i);
    }
    /*Get All sequence*/
    computePoint* start = new computePoint(warpOutput, avail, inputType, (int)(inputType.size()));
    computeSearchTree tree(start);
    vector<GPFunctionTreePoint*> queue;
    bool find = false;
    while (tree.searchOne(&queue))
    {
        GPASSERT(1==queue.size());
        /*Make Sure the result use all inputs*/
        GPFunctionTreePoint* _point = queue[0];
        AUTOCLEAN(_point);
        auto real_inp = reshape(_point->getInputTypes());
        if (!_ValidateInputs(real_inp, inputType))
        {
            continue;
        }
        find = true;
        _point->addRef();
        break;
    }
    if (!find)
    {
        return NULL;
    }
    GPASSERT(1 == queue.size());
    GPFunctionTreePoint* originpoint = queue[0];
    /*Make Input Map*/
    auto real_inputs = reshape(originpoint->getInputTypes());
    map<int, int> inputPosMap;
    vector<pair<const IStatusType*, bool>> usedInputs;
    for (size_t i=0; i<inputType.size(); ++i)
    {
        usedInputs.push_back(make_pair(inputType[i], false));
    }
    for (size_t i=0; i<real_inputs.size(); ++i)
    {
        /*Find not used pos firstly*/
        size_t pos;
        auto neededtype = real_inputs[i];
        for (pos=0; pos < usedInputs.size(); ++pos)
        {
            if ((!usedInputs[pos].second) && neededtype == usedInputs[pos].first)
            {
                usedInputs[pos].second = true;
                break;
            }
        }
        if (pos >= usedInputs.size())
        {
            /*Not found unused one, find first one instead*/
            for (pos=0; pos < usedInputs.size(); ++pos)
            {
                if (neededtype == usedInputs[pos].first)
                {
                    inputPosMap.insert(make_pair(i, pos));
                    break;
                }
            }
        }
        GPASSERT(pos < usedInputs.size());
    }
    originpoint->mapInput(inputPosMap);
    return originpoint;
}


static std::vector<GPPtr<GPFunctionTreePoint>> _searchAllFunction(const std::vector<const IStatusType*>& outputType, const std::vector<const IStatusType*>& inputType, const GPProducerUtils& utils, int depth)
{
    vector<vector<const GPProducerUtils::func*> > warpOutput;
    _findMatchedFuncton(warpOutput, outputType, inputType, utils);
    warpOutput = _filterOutputType(warpOutput, inputType);
    if (warpOutput.empty())
    {
        return std::vector<GPPtr<GPFunctionTreePoint>>();
    }
    vector<int> avail;
    for (int i=0; i<warpOutput.size(); ++i)
    {
        avail.push_back(i);
    }
    /*Get All sequence*/
    computePoint* start = new computePoint(warpOutput, avail, inputType, depth);
    computeSearchTree tree(start);
    auto contents = tree.searchAll();
    std::vector<GPPtr<GPFunctionTreePoint>> result;
    for (auto c : contents)
    {
        GPASSERT(1 == c.size());
        auto real_inputTypes = c[0]->getInputTypes();
        bool valid = true;
        if (real_inputTypes.size() != inputType.size())
        {
            valid = false;
        }
        else
        {
            for (int i=0; i<inputType.size(); ++i)
            {
                if (real_inputTypes[i]!=inputType[i])
                {
                    valid = false;
                    break;
                }
            }
        }
        if (valid)
        {
            result.push_back(c[0]);
            c[0]->addRef();
        }
        c[0]->decRef();
    }
    return result;
}


GPBasicAdaptor::GPBasicAdaptor(const GPFunctionDataBase* database):mUtils(database),mDataBase(database)
{
}

GPBasicAdaptor::~GPBasicAdaptor()
{
    
}

GPFunctionTreePoint* GPBasicAdaptor::_revert(const GP__Point* src) const
{
    GPASSERT(NULL!=src);
    GPFunctionTreePoint* point = NULL;
    switch (src->type)
    {
        case GP__POINT__TYPE__FUNCTION:
        {
            auto f = mDataBase->vQueryFunctionByShortName(src->content);
            point = new GPFunctionTreePoint(f);
        }
            break;
        case GP__POINT__TYPE__INPUT:
        {
            std::string variable = src->content;
            point = new GPFunctionTreePoint(GPFunctionTreePoint::INPUT, _loadXn(variable));
        }
            break;
        case GP__POINT__TYPE__OUTPUT:
        {
            GPASSERT(false);
        }
            break;
        default:
            break;
    }
    GPASSERT(NULL!=point);
    for (int i=0; i<src->n_input_variable; ++i)
    {
        auto p = _revert(src->input_variable[i]);
        point->addPoint(p);
    }
    return point;
}

static GP__Point* _copyFunctionPoint(const GPFunctionTreePoint* src)
{
    GPASSERT(NULL!=src);
    GP__Point* result = (GP__Point*)::malloc(sizeof(GP__Point));
    gp__point__init(result);
    result->n_input_variable = src->getChildrenNumber();
    result->input_variable = (GP__Point**)::malloc(sizeof(GP__Point*)*src->getChildrenNumber());
    for (int i=0; i<src->getChildrenNumber(); ++i)
    {
        auto src_p = GPCONVERT(const GPFunctionTreePoint, src->getChild(i));
        result->input_variable[i] = _copyFunctionPoint(src_p);
    }
    //Copy content
    switch (src->type())
    {
        case GPFunctionTreePoint::FUNCTION:
            result->type = GP__POINT__TYPE__FUNCTION;
            result->content = ::strdup(src->data().pFunc->shortname.c_str());
            break;
        case GPFunctionTreePoint::INPUT:
        {
            result->type = GP__POINT__TYPE__INPUT;
            ostringstream variableName;
            variableName << "x" << src->data().iInput;
            result->content = ::strdup(variableName.str().c_str());
        }
            break;
        case GPFunctionTreePoint::OUTPUT:
        {
            GPASSERT(false);//TODO
            result->type = GP__POINT__TYPE__OUTPUT;
            result->n_output_names = src->getChildrenNumber();
            for (int i=0; i<src->getChildrenNumber(); ++i)
            {
                ostringstream variableName;
                variableName << "y" << i;
                result->output_names[i] = ::strdup(variableName.str().c_str());
            }
        }
            break;
        default:
            GPASSERT(false);
            break;
    }
    
    return result;
}

GP__Point* GPBasicAdaptor::_expand(const GP__ADF* adf) const
{
    GPASSERT(NULL!=adf);
    vector<const IStatusType*> outputType;
    GPASSERT(adf->n_output_types == 1);//TODO
    for (int i=0; i<adf->n_output_types; ++i)
    {
        outputType.push_back(mDataBase->vQueryType(adf->output_types[i]));
    }
    vector<const IStatusType*> inputType;
    for (int i=0; i<adf->n_input_types; ++i)
    {
        inputType.push_back(mDataBase->vQueryType(adf->input_types[i]));
    }
    GPPtr<GPFunctionTreePoint> point = _createOnePoint(outputType, inputType, mUtils);
    return _copyFunctionPoint(point.get());
}


bool GPBasicAdaptor::vExpand(GP__PointGroup* origin) const
{
    GPASSERT(NULL!=origin);
    for (int i=0; i<origin->n_adfs; ++i)
    {
        auto adf = origin->adfs[i];
        GPASSERT(NULL!=adf && NULL == adf->realization);
        adf->realization = _expand(adf);
    }
    return true;
}

template <typename T>
static T _random(float p, const std::vector<T>& queue)
{
    size_t n = p*queue.size();
    if (n >= queue.size())
    {
        n = queue.size()-1;
    }
    return queue[n];
}

int GPBasicAdaptor::vMapStructure(GP__PointGroup* target, const double* paramters, int n_paramters) const
{
    const int BATCH = 3;
    const int DEFAULT_DEPTH = 3;
    GPASSERT(NULL!=target);
    auto paramterNumber = (int)target->n_adfs*BATCH;
    if (NULL == paramters)
    {
        return paramterNumber;
    }
    GPASSERT(n_paramters == paramterNumber);
    for (int i=0; i<target->n_adfs; ++i)
    {
        auto adf = target->adfs[i];
        GPASSERT(NULL!=adf && NULL!=adf->realization);
        auto p = paramters + BATCH*i;
        auto p0 = p[0];
        auto p1 = p[1];
        auto p2 = p[2];
        if (p0  < 0.3)
        {
            continue;
        }
        GPPtr<GPFunctionTreePoint> point = _revert(adf->realization);
        auto allPoints = point->display();
        GPFunctionTreePoint* origin_point = GPFORCECONVERT(GPFunctionTreePoint, _random(p1, allPoints));
        auto inputsTotal = origin_point->getInputTypes();
        std::vector<const IStatusType*> inputs;
        std::map<int, int> inputMap;
        int cur = 0;
        for (auto& iter : inputsTotal)
        {
            inputs.push_back(iter.second);
            inputMap.insert(std::make_pair(cur++, iter.first));
        }
        auto outputs = origin_point->data().pFunc->outputType;
        auto depth = DEFAULT_DEPTH;
        if (adf->has_limit_depth)
        {
            depth = adf->limit_depth;
        }
        auto treepoints = _searchAllFunction(outputs, inputs, mUtils, depth);
        if (treepoints.empty())
        {
            continue;
        }
        auto replace_point = _random(p2, treepoints);
        replace_point->mapInput(inputMap);
        const bool debugVary = false;
        if (debugVary)
        {
            std::ostringstream formula;
            origin_point->render(formula);
            FUNC_PRINT_ALL(formula.str().c_str(), s);
        }
        origin_point->copyFrom(replace_point.get());
        
        gp__point__free_unpacked(adf->realization, NULL);
        adf->realization = _copyFunctionPoint(point.get());
    }
    return paramterNumber;
}
