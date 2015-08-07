#include "utils/GPStringHelper.h"
static bool isDivider(char c)
{
    return (c == ' ') || (c == '\t')||(c == ','||(c == '\n'));
}
std::vector<std::string>  GPStringHelper::divideString(const std::string& line)
{
#define LENGTH_OUT(x) if (x >= line.size()) {break;}
    std::vector<std::string> result;
    int cur =0;
    while(true)
    {
        for (;isDivider(line[cur]) && cur < line.size();++cur);
        LENGTH_OUT(cur);
        int sta = cur;
        for (++cur;(!isDivider(line[cur]))&& cur < line.size();++cur);
        int fin = cur;
        result.push_back(line.substr(sta, fin-sta));
        LENGTH_OUT(cur);
        ++cur;
    }
#undef LENGTH_OUT
    return result;
}
