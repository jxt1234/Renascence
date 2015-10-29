#include "frontend/GPFunctionFrontEndProducer.h"
#include "frontend/GPFormulaTree.h"
#include "math/carryGroup2.h"
#include "recurse_tree.h"

using namespace std;

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

GPFunctionTree* GPFunctionFrontEndProducer::vCreateOneFunction(const std::vector<const IStatusType*>& outputType, const std::vector<const IStatusType*>& inputType) const
{
    /*Find all available output function*/
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
    vector<GPFunctionTree*> queue = tree.searchOne();
    GPASSERT(1 == queue.size());
    return queue[0];
}
std::vector<GPFunctionTree*> GPFunctionFrontEndProducer::vCreateAllFunction(const std::vector<const IStatusType*>& outputType, const std::vector<const IStatusType*>& inputType) const
{
    vector<vector<const GPProducerUtils::func*> > warpOutput;
    _findMatchedFuncton(warpOutput, outputType, inputType, mUtils);
    warpOutput = _filterOutputType(warpOutput, inputType);
    if (warpOutput.empty())
    {
        std::vector<GPFunctionTree*> t;
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
    std::vector<GPFunctionTree*> result;
    for (auto c : contents)
    {
        GPASSERT(1 == c.size());
        result.push_back(c[0]);
    }
    return result;
}
class formulaCopy:public GPAbstractPoint::IPointCopy
{
public:
    formulaCopy(const GPFunctionDataBase* base):mBase(base){}
    virtual ~formulaCopy(){}
    virtual GPAbstractPoint* copy(GPAbstractPoint* src)
    {
        GPFormulaTreePoint* point = (GPFormulaTreePoint*)(src);
        GPASSERT(NULL!=point);
        if (GPFormulaTreePoint::NUM == point->type())
        {
            std::string s = point->name();
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
            return new GPFunctionTree(NULL, sum);
        }
        const GPFunctionDataBase::function* f = mBase->vQueryFunctionByShortName(point->name());
        if (NULL == f)
        {
            f = mBase->vQueryFunction(point->name());
        }
        GPASSERT(NULL!=f);
        return new GPFunctionTree(f, -1);
    }
private:
    const GPFunctionDataBase* mBase;
};

GPFunctionTree* GPFunctionFrontEndProducer::vCreateFromFormula(const std::string& formula) const
{
    GPFormulaTree tree;
    tree.setFormula(formula);
    formulaCopy copy(mUtils.getOriginBase());
    auto a = GPAbstractPoint::deepCopy(tree.root(), &copy);
    return (GPFunctionTree*)a;
}


GPFunctionFrontEndProducer::GPFunctionFrontEndProducer(const GPFunctionDataBase* base):mUtils(base)
{
}
GPFunctionFrontEndProducer::~GPFunctionFrontEndProducer()
{
}
