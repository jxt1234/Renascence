#include "head.h"
#include "math/GPFormula.h"
using namespace std;

GPFormula::GPFormula(const std::string& formula)
{
    vector<int> func;
    vector<int> number;
    analysis(func, number, formula);
    assert(func.size() == number.size());
    for (int i=0; i<func.size(); ++i)
    {
        mQueue.push_back(func[i]);
        mQueue.push_back(0);//status, deceperate
        mQueue.push_back(number[i]);
    }
}

GPFormula::~GPFormula()
{
}

void GPFormula::analysis(std::vector<int>& func, std::vector<int>& number, const std::string& formula)
{
}
