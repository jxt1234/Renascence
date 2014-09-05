#include "head.h"
#include "math/GPFormula.h"
#include <list>
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

/*TODO support double operator such as a+b*/
void GPFormula::analysis(std::vector<int>& func, std::vector<int>& number, const std::string& formula)
{
    const std::string& s = formula;
    vector<int> left;
    vector<int> right;
    for (int i=0; i<s.size(); ++i)
    {
        char c= s[i];
        if (c == '(') left.push_back(i);
        if (c == ')') right.push_back(i);
    }
    assert(left.size() == right.size());
    /*Find all function, the function will be in the left of '('*/
    for (int i=0; i<left.size(); ++i)
    {
        int cur = left[i];
        /*Flip space and tab*/
        while(0<=cur)
        {
            if (s[cur] != ' ' && s[cur] != '\t')
            {
                break;
            }
            --cur;
        }
        /*Get the function number*/
        int funcNo = 0;
        int _s = 1;
        while(0<=cur)
        {
            char c = s[cur];
            if (c<'0' || c>'9')
            {
                break;
            }
            int bit = c - '0';
            funcNo += bit * _s;
            --cur;
            _s*=10;
        }
        func.push_back(funcNo);
    }
    /*Get All functions' children number*/
    // If exist ',', the number is the amount of ',' +1, else, the number is 1(has content) or 0 (no content)
    int l_cur = 0;
    int r_cur = 0;
    /*TODO Optimize it*/
    while (r_cur < left.size())
    {
        int r = right[r_cur];
        while(left[l_cur] < r && l_cur < left.size())
        {
            l_cur++;
        }
        l_cur -=1;
    }
}
