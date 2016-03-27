#include "frontend/GPFunctionFrontEndProducer.h"
#include "frontend/GPFormulaTree.h"
#include "math/carryGroup.h"
#include "recurse_tree.h"
#include <algorithm>
#include <sstream>
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


GPFunctionTree* GPFunctionFrontEndProducer::vCreateOneFunction(const std::vector<const IStatusType*>& outputType, const std::vector<const IStatusType*>& inputType) const
{
    return new GPFunctionTree(_createOnePoint(outputType, inputType, mUtils));
}

static std::vector<GPFunctionTreePoint*> _searchAllFunction(const std::vector<const IStatusType*>& outputType, const std::vector<const IStatusType*>& inputType, const GPProducerUtils& utils, int depth)
{
    vector<vector<const GPProducerUtils::func*> > warpOutput;
    _findMatchedFuncton(warpOutput, outputType, inputType, utils);
    warpOutput = _filterOutputType(warpOutput, inputType);
    if (warpOutput.empty())
    {
        std::vector<GPFunctionTreePoint*> t;
        return t;
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
    std::vector<GPFunctionTreePoint*> result;
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

std::vector<GPFunctionTree*> GPFunctionFrontEndProducer::vCreateAllFunction(const std::vector<const IStatusType*>& outputType, const std::vector<const IStatusType*>& inputType, int depth) const
{
    auto treepoints = _searchAllFunction(outputType, inputType, mUtils, depth);
    std::vector<GPFunctionTree*> result;
    for (auto c : treepoints)
    {
        result.push_back(new GPFunctionTree(c));
    }
    return result;
}
class formulaCopy:public GPAbstractPoint::IPointCopy
{
public:
    formulaCopy(const GPFunctionDataBase* base, const GPProducerUtils& utils, const std::vector<const IStatusType*>& inputTypes):mBase(base), mUtils(utils), mInputTypes(inputTypes){}
    virtual ~formulaCopy(){}
    const std::map<string, std::pair<GPFunctionTreePoint*, std::vector<GPFunctionTreePoint*>>>& getMutable() const {return mADFs;}
    GPFunctionTreePoint* newAdf(GPFormulaTreePoint* point)
    {
        GPASSERT(!mInputTypes.empty());
        GPASSERT(point->getChildrenNumber()>=1);
        auto adfname = ((const GPFormulaTreePoint*)(point->getChild(0)))->name();
        if (mADFs.find(adfname)!=mADFs.end())
        {
            auto result = mADFs.find(adfname)->second.first;
            result->addRef();
            return result;
        }
        //Find output
        std::vector<const IStatusType*> outputType;
        auto father = point->father();
        int n = father->getChildrenNumber();
        int pos = -1;
        for (int i=0; i<n; ++i)
        {
            if (father->getChild(i) == point)
            {
                pos = i;
                break;
            }
        }
        GPASSERT(-1!=pos);
        outputType.push_back(mBase->vQueryFunctionByShortName(father->name())->inputType[pos]);
        //Find Input
        std::vector<const IStatusType*> inputType;
        std::map<int, GPFunctionTreePoint*> inputFunc;
        std::map<int, int> inputmaps;
        for (int i=1; i<point->getChildrenNumber(); ++i)
        {
            GPFormulaTreePoint* p = (GPFormulaTreePoint*)(point->getChild(i));
            if (GPFormulaTreePoint::NUM == p->type())
            {
                int pos = _loadXn(p->name());
                GPASSERT(pos < mInputTypes.size());
                inputType.push_back(mInputTypes[pos]);
                inputmaps.insert(make_pair(i-1, pos));
            }
            else if (GPFormulaTreePoint::OPERATOR == p->type())
            {
                inputType.push_back(mBase->vQueryFunctionByShortName(p->name())->outputType[0]);
                GPFunctionTreePoint* funcPoint = (GPFunctionTreePoint*)GPAbstractPoint::deepCopy(p, this);
                funcPoint->valid();
                inputFunc.insert(std::make_pair(i-1, funcPoint));
            }
            else
            {
                GPASSERT(0);
            }
        }
        auto originpoint = _createOnePoint(outputType, inputType, mUtils);
        GPASSERT(NULL!=originpoint);
        /*Second Map for not-inorder input*/
        originpoint->mapInput(inputmaps);
        originpoint->mapInput(inputFunc);
        
        /*The FuncPoint will be ref by mapInput, deRef them now*/
        std::vector<GPFunctionTreePoint*> immutable;
        for (auto& iter : inputFunc)
        {
            immutable.push_back(iter.second);
            iter.second->decRef();
        }
        mADFs.insert(std::make_pair(adfname, std::make_pair(originpoint, immutable)));
        originpoint->valid();
        return originpoint;
    }
    GPFunctionTreePoint* newParallel(GPFormulaTreePoint* point)
    {
        GPASSERT(point->getChildrenNumber() >= 2);
        GPFunctionTreePoint* result = NULL;
        int type = 0;
        if (point->name() == "REDUCE")
        {
            type = 1;
        }
        else if (point->name() == "MAP")
        {
            type = 0;
        }
        else
        {
            GPASSERT(0);
        }
        result = new GPFunctionTreePoint(GPFunctionTreePoint::PARALLEL, type);
        {
            GPFunctionTreePoint* input_f = new GPFunctionTreePoint("INPUT");
            result->addPoint(input_f);
            GPFormulaTreePoint* input = (GPFormulaTreePoint*)(point->getChild(0));
            for (int i=0; i<input->getChildrenNumber(); ++i)
            {
                input_f->addPoint(GPAbstractPoint::deepCopy((GPAbstractPoint*)(input->getChild(i)), this));
            }
        }
        {
            result->addPoint(GPAbstractPoint::deepCopy((GPAbstractPoint*)(point->getChild(1)), this));
        }
        if (point->getChildrenNumber()>2)
        {
            GPFunctionTreePoint* keymap = new GPFunctionTreePoint("KEYMAP");
            result->addPoint(keymap);
            GPFormulaTreePoint* keymap_origin = (GPFormulaTreePoint*)(point->getChild(2));
            GPASSERT(keymap_origin->getChildrenNumber() == 2);
            //TODO: consider input key
            {
                GPFormulaTreePoint* outputkey = (GPFormulaTreePoint*)keymap_origin->getChild(1);
                auto nodes = outputkey->display();
                for (auto p : nodes)
                {
                    auto pp = (GPFormulaTreePoint*)p;
                    if (pp->type() == GPFormulaTreePoint::NUM)
                    {
                        GPFunctionTreePoint* par = new GPFunctionTreePoint(pp->name());
                        keymap->addPoint(par);
                    }
                }
                
            }
        }
        if (point->getChildrenNumber()>3)
        {
            GPFormulaTreePoint* condition = (GPFormulaTreePoint*)(point->getChild(3));
            result->addPoint(new GPFunctionTreePoint(condition->name()));
        }
        
        
        return result;
    }

    virtual GPAbstractPoint* copy(GPAbstractPoint* src, bool& needcopyChild)
    {
        GPFormulaTreePoint* point = (GPFormulaTreePoint*)(src);
        GPASSERT(NULL!=point);
        switch (point->type())
        {
            case GPFormulaTreePoint::NUM:
            {
                std::string s = point->name();
                needcopyChild = false;
                return new GPFunctionTreePoint(GPFunctionTreePoint::INPUT, _loadXn(s));
            }
            case GPFormulaTreePoint::ADF:
            {
                needcopyChild = false;
                return newAdf(point);
            }
            case GPFormulaTreePoint::OPERATOR:
            {
                needcopyChild = true;
                const GPFunction* f = mBase->vQueryFunctionByShortName(point->name());
                if (NULL == f)
                {
                    f = mBase->vQueryFunction(point->name());
                }
                GPASSERT(NULL!=f);
                return new GPFunctionTreePoint(f);
            }
            case GPFormulaTreePoint::PARALLEL:
                needcopyChild = false;
                return newParallel(point);
            default:
                GPASSERT(0);
                break;
        }
        return NULL;
    }
private:
    const GPFunctionDataBase* mBase;
    const GPProducerUtils& mUtils;
    std::map<string, std::pair<GPFunctionTreePoint*, std::vector<GPFunctionTreePoint*>>> mADFs;
    const std::vector<const IStatusType*>& mInputTypes;
};

GPFunctionTree* GPFunctionFrontEndProducer::vCreateFromFormula(const std::string& formula, const std::vector<const IStatusType*> inputTypes) const
{
    GPFormulaTree tree;
    tree.setFormula(formula);
    formulaCopy copy(mUtils.getOriginBase(), mUtils, inputTypes);
    GPPtr<GPFunctionTreePoint> a = (GPFunctionTreePoint*)(GPAbstractPoint::deepCopy(tree.root(), &copy));
    GPFunctionTree* result = new GPFunctionTree(a, false);
    for (auto& adfiter : copy.getMutable())
    {
        result->addVariableSubTree(adfiter.second.first, adfiter.second.second, adfiter.first);
    }
    return result;
}


GPFunctionFrontEndProducer::GPFunctionFrontEndProducer(const GPFunctionDataBase* base):mUtils(base)
{
}
GPFunctionFrontEndProducer::~GPFunctionFrontEndProducer()
{
}


static std::map<GPFunctionTreePoint*, std::map<int, GPFunctionTreePoint*>> makeReplace(const std::map<GPFunctionTreePoint*, std::vector<GPFunctionTreePoint*>>& variable)
{
    std::map<GPFunctionTreePoint*, std::map<int, GPFunctionTreePoint*>> result;
    /*Find max input pos*/
    for (auto iter : variable)
    {
        GPFunctionTreePoint* root = iter.first;
        int replaceId = root->maxInputPos()+1;
        std::map<int, GPFunctionTreePoint*> temp_result;
        for (auto p : iter.second)
        {
            GPPtr<GPFunctionTreePoint> replace = new GPFunctionTreePoint(GPFunctionTreePoint::INPUT, replaceId);
            temp_result.insert(std::make_pair(replaceId, p));
            p->addRef();//Ref the point because replace wil deRef it
            root->replace(p, replace.get());
            replaceId++;
        }
        result.insert(std::make_pair(root, temp_result));
    }
    return result;
}
static void restore(std::map<GPFunctionTreePoint*, std::map<int, GPFunctionTreePoint*>>& result)
{
    for (auto iter : result)
    {
        GPFunctionTreePoint* root = iter.first;
        root->mapInput(iter.second);
        for (auto p : iter.second)
        {
            p.second->decRef();
        }
    }
}

class AutoRestore:public GPRefCount
{
public:
    AutoRestore(std::map<GPFunctionTreePoint*, std::map<int, GPFunctionTreePoint*>>& map):mMap(map)
    {
    }
    ~AutoRestore()
    {
        restore(mMap);
    }
private:
    std::map<GPFunctionTreePoint*, std::map<int, GPFunctionTreePoint*>>& mMap;
};


int GPFunctionFrontEndProducer::vMapStructure(GPFunctionTree* tree, GPParameter* para, int depth, bool& changed) const
{
    GPASSERT(NULL!=tree);
    changed = false;
    const int magic_number = 3;
    if (NULL == para)
    {
        return magic_number;
    }
    GPFLOAT p0 = para->get(0);
    GPFLOAT p1 = para->get(1);
    GPFLOAT p2 = para->get(2);
    
    if (p0 < 0.3)//TODO
    {
        return magic_number;
    }
    /*Search All Variable Points*/
    auto& variable_points = tree->getVariable();
    if (variable_points.empty())
    {
        return magic_number;
    }
    auto cacheMaps = makeReplace(variable_points);
    AutoRestore _r(cacheMaps);
    std::vector<GPFunctionTreePoint*> allpoints;
    /*FIXME inputmap and function save*/
    for (auto p : variable_points)
    {
        for (auto pp : p.first->display())
        {
            if (pp->getChildrenNumber()>0)
            {
                allpoints.push_back((GPFunctionTreePoint*)pp);
            }
        }
    }
    if (allpoints.empty())
    {
        return magic_number;
    }
    changed = true;
    size_t n = p1*allpoints.size();
    if (n >= allpoints.size())
    {
        n = allpoints.size()-1;
    }
    GPFunctionTreePoint* origin_point = allpoints[n];
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
    auto treepoints = _searchAllFunction(outputs, inputs, mUtils, depth);
    if (treepoints.empty())
    {
        changed = false;
        return magic_number;
    }
    n = p2*treepoints.size();
    if (n >= treepoints.size())
    {
        n = treepoints.size()-1;
    }
    auto replace_point = treepoints[n];
    replace_point->mapInput(inputMap);
    const bool debugVary = false;
    if (debugVary)
    {
        std::ostringstream formula;
        origin_point->render(formula);
        FUNC_PRINT_ALL(formula.str().c_str(), s);
    }
    origin_point->copyFrom(replace_point);
    if (debugVary)
    {
        std::ostringstream formula;
        origin_point->render(formula);
        FUNC_PRINT_ALL(formula.str().c_str(), s);
    }
    
    /*replace_point is in treepoints, don't need to deRef*/
    for (auto p : treepoints)
    {
        p->decRef();
    }
    
    return magic_number;
}

