#include "frontend/GPFunctionFrontEndProducer.h"
#include "frontend/GPFormulaTree.h"
#include "math/carryGroup2.h"
#include "recurse_tree.h"
#include <algorithm>
using namespace std;

static int _loadXn(const string& s)
{
    int n = (int)(s.size());
    int sum = 0;
    for(int i=n-1; i>=0; --i)
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

/*Like This Format: (TrPackageCompse:1)[TrBmp:x5,TrBmp:x6]*/
static pair<string, int> _loadADF(const string& s, vector<pair<string, int>>& variables)
{
    size_t cur = 1;
    while (s[cur]!=':') cur++;
    string func = s.substr(1, cur-1);
    size_t sta = cur+1;
    cur = sta+1;
    while (s[cur]!=')') cur++;
    int number = _loadXn(s.substr(sta, cur-sta+1));
    sta = cur+2;
    cur = sta;
    while (s[cur]!=']')
    {
        while (s[cur]!=':') cur++;
        string type = s.substr(sta, cur-sta);
        sta = cur+1;
        cur = sta;
        while (s[cur]!=']' && s[cur]!=',') cur++;
        int x = _loadXn(s.substr(sta, cur-sta));
        sta = cur+1;
        variables.push_back(make_pair(type, x));
    }
    
    return make_pair(func, number);
}


static vector<vector<const GPProducerUtils::func*> > _filterOutputType(const vector<vector<const GPProducerUtils::func*> >& origin, const std::vector<const IStatusType*>& inputType)
{
    vector<vector<const GPProducerUtils::func*> > result;
    for (auto p : origin)
    {
        vector<const IStatusType*> inputs;
        for (auto f : p)
        {
            inputs.insert(inputs.end(), f->inputs.begin(), f->inputs.end());
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
    carryGroup2<const GPProducerUtils::func*> group;
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

static GPFunctionTreePoint* _createOnePoint(const std::vector<const IStatusType*>& outputType, const std::vector<const IStatusType*>& inputType, const GPProducerUtils& mUtils)
{
    vector<vector<const GPProducerUtils::func*> > warpOutput;
    _findMatchedFuncton(warpOutput, outputType, inputType, mUtils);
    warpOutput = _filterOutputType(warpOutput, inputType);
    if (warpOutput.empty())
    {
        return NULL;
    }
    vector<int> avail(1,warpOutput.size()-1);
    /*Get All sequence*/
    computePoint* start = new computePoint(warpOutput, avail, inputType);
    computeSearchTree tree(start);
    vector<GPFunctionTreePoint*> queue;
    bool find = false;
    while (tree.searchOne(&queue))
    {
        GPASSERT(1==queue.size());
        /*Make Sure the result use all inputs*/
        GPFunctionTreePoint* _point = queue[0];
        AUTOCLEAN(_point);
        auto real_inp = _point->getInputTypes();
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
    auto real_inputs = originpoint->getInputTypes();
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

static std::vector<GPFunctionTreePoint*> _searchAllFunction(const std::vector<const IStatusType*>& outputType, const std::vector<const IStatusType*>& inputType, const GPProducerUtils& utils)
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
    computePoint* start = new computePoint(warpOutput, avail, inputType);
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

std::vector<GPFunctionTree*> GPFunctionFrontEndProducer::vCreateAllFunction(const std::vector<const IStatusType*>& outputType, const std::vector<const IStatusType*>& inputType) const
{
    auto treepoints = _searchAllFunction(outputType, inputType, mUtils);
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
    formulaCopy(const GPFunctionDataBase* base, const GPProducerUtils& utils):mBase(base), mUtils(utils){}
    virtual ~formulaCopy(){}
    virtual GPAbstractPoint* copy(GPAbstractPoint* src)
    {
        GPFormulaTreePoint* point = (GPFormulaTreePoint*)(src);
        GPASSERT(NULL!=point);
        if (GPFormulaTreePoint::NUM == point->type())
        {
            std::string s = point->name();
            return new GPFunctionTreePoint(NULL, _loadXn(s));
        }
        else if (GPFormulaTreePoint::ADF == point->type())
        {
            vector<pair<string, int>> inputMap;
            auto outputMap = _loadADF(point->name(), inputMap);
            std::vector<const IStatusType*> outputType;
            outputType.push_back(mBase->vQueryFunctionByShortName(outputMap.first)->inputType[outputMap.second]);
            std::vector<const IStatusType*> inputType;
            for (size_t i=0; i<inputMap.size(); ++i)
            {
                inputType.push_back(mBase->vQueryType(inputMap[i].first));
            }
            auto originpoint = _createOnePoint(outputType, inputType, mUtils);
            /*Second Map for not-inorder input*/
            map<int, int> inputmaps;
            for (size_t i=0; i<inputMap.size(); ++i)
            {
                inputmaps.insert(make_pair(i, inputMap[i].second));
            }
            originpoint->mapInput(inputmaps);
            return originpoint;
        }
        const GPFunctionDataBase::function* f = mBase->vQueryFunctionByShortName(point->name());
        if (NULL == f)
        {
            f = mBase->vQueryFunction(point->name());
        }
        GPASSERT(NULL!=f);
        return new GPFunctionTreePoint(f, -1);
    }
private:
    const GPFunctionDataBase* mBase;
    const GPProducerUtils& mUtils;
};

GPFunctionTree* GPFunctionFrontEndProducer::vCreateFromFormula(const std::string& formula) const
{
    GPFormulaTree tree;
    tree.setFormula(formula);
    formulaCopy copy(mUtils.getOriginBase(), mUtils);
    auto a = (GPFunctionTreePoint*)(GPAbstractPoint::deepCopy(tree.root(), &copy));
    return new GPFunctionTree(a);
}


GPFunctionFrontEndProducer::GPFunctionFrontEndProducer(const GPFunctionDataBase* base):mUtils(base)
{
}
GPFunctionFrontEndProducer::~GPFunctionFrontEndProducer()
{
}

int GPFunctionFrontEndProducer::vMapStructure(GPFunctionTree* tree, GPParameter* para, bool& changed) const
{
    GPASSERT(NULL!=tree);
    
    const int magic_number = 3;
    if (NULL == para)
    {
        changed = false;
        return magic_number;
    }
    PFLOAT p0 = para->get(0);
    PFLOAT p1 = para->get(1);
    PFLOAT p2 = para->get(2);
    
    if (p0 < 0.3)
    {
        changed = false;
        return magic_number;
    }
    std::vector<GPFunctionTreePoint*> allpoints;
    /*Search All Variable Points*/
    {
        auto variable_points = tree->getVariable();
        for (auto p : variable_points)
        {
            for (auto pp : p->display())
            {
                /*Don't modified root*/
                if (pp != tree->root())
                {
                    allpoints.push_back((GPFunctionTreePoint*)pp);
                }
            }
        }
    }
    if (allpoints.empty())
    {
        changed = false;
        return magic_number;
    }
    size_t n = p1*allpoints.size();
    if (n >= allpoints.size())
    {
        n = allpoints.size()-1;
    }
    changed = true;
    GPFunctionTreePoint* origin_point = allpoints[n];
    auto inputs = origin_point->getInputTypes();
    auto outputs = origin_point->function()->outputType;
    auto treepoints = _searchAllFunction(outputs, inputs, mUtils);
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
    tree->root()->replace(origin_point, replace_point);
    
    for (auto p : treepoints)
    {
        p->decRef();
    }
    
    return magic_number;
}

