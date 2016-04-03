/******************************************************************
   Copyright 2016, Jiang Xiao-tang

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

std::string GPStringHelper::cleanString(const std::string& input)
{
    if (input.size()==0)
    {
        return "";
    }
    int sta = -1;
    int fin = -1;
    for (int i=0; i<input.size(); ++i)
    {
        if (!isDivider(input[i]))
        {
            sta = i;
            break;
        }
    }
    for (int i=input.size()-1; i>=0; --i)
    {
        if (!isDivider(input[i]))
        {
            fin = i;
            break;
        }
    }
    if (sta == -1 || fin == -1)
    {
        return "";
    }
    return input.substr(sta, fin-sta+1);
}
