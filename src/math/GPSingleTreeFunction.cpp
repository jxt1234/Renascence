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
#include "math/GPSingleTreeFunction.h"
#include <math.h>
int GPSingleTreeFunction::func(const std::string& name)
{
if (name == ">=") return 0;
if (name == "<=") return 1;
if (name == "==") return 2;
if (name == "!=") return 3;
if (name == "&&") return 4;
if (name == "||") return 5;
if (name == "*") return 6;
if (name == "X") return 7;
if (name == "+") return 8;
if (name == "-") return 9;
if (name == "/") return 10;
if (name == "%") return 11;
if (name == ">") return 12;
if (name == "<") return 13;
if (name == "!") return 14;
return -1;
}
std::string GPSingleTreeFunction::query(int func)
{
if (func == 0) return ">=";
if (func == 1) return "<=";
if (func == 2) return "==";
if (func == 3) return "!=";
if (func == 4) return "&&";
if (func == 5) return "||";
if (func == 6) return "*";
if (func == 7) return "X";
if (func == 8) return "+";
if (func == 9) return "-";
if (func == 10) return "/";
if (func == 11) return "%";
if (func == 12) return ">";
if (func == 13) return "<";
if (func == 14) return "!";
return "";
}
int GPSingleTreeFunction::inputNumber(int func)
{
if (func == 0) return 2;
if (func == 1) return 2;
if (func == 2) return 2;
if (func == 3) return 2;
if (func == 4) return 2;
if (func == 5) return 2;
if (func == 6) return 2;
if (func == 7) return 2;
if (func == 8) return 2;
if (func == 9) return 2;
if (func == 10) return 2;
if (func == 11) return 2;
if (func == 12) return 2;
if (func == 13) return 2;
if (func == 14) return 2;
return -1;
}
unsigned int GPSingleTreeFunction::compute(int func, unsigned int* x)
{
if (func == 0) return (x[0]>=x[1]?1:0);
if (func == 1) return (x[0]<=x[1]?1:0);
if (func == 2) return (x[0]==x[1]?1:0);
if (func == 3) return (x[0]!=x[1]?1:0);
if (func == 4) return ((x[0]>0&&x[1]>0)?1:0);
if (func == 5) return ((x[0]>0||x[1]>0)?1:0);
if (func == 6) return (x[0]*x[1]);
if (func == 7) return (x[0]*x[1]);
if (func == 8) return (x[0]+x[1]);
if (func == 9) return (x[0]-x[1]);
if (func == 10) return (x[0]/x[1]);
if (func == 11) return (x[0]%x[1]);
if (func == 12) return (x[0]>x[1]?1:0);
if (func == 13) return (x[0]<x[1]?1:0);
if (func == 14) return (x[0]<=0?1:0);
return 0;
}
int GPSingleTreeFunction::priority(int func)
{
if (func == 0) return 2;
if (func == 1) return 2;
if (func == 2) return 2;
if (func == 3) return 2;
if (func == 4) return 1;
if (func == 5) return 1;
if (func == 6) return 5;
if (func == 7) return 5;
if (func == 8) return 3;
if (func == 9) return 3;
if (func == 10) return 5;
if (func == 11) return 5;
if (func == 12) return 2;
if (func == 13) return 2;
if (func == 14) return 1;
return 0;
}
int GPSingleTreeFunction::searchFunction(int sta, const std::string& formula)
{
int len = (int)(formula.size()) - sta;
if ((len >= 2) && (formula[sta+0]) == '>' && (formula[sta+1]) == '=') return 2;
if ((len >= 2) && (formula[sta+0]) == '<' && (formula[sta+1]) == '=') return 2;
if ((len >= 2) && (formula[sta+0]) == '=' && (formula[sta+1]) == '=') return 2;
if ((len >= 2) && (formula[sta+0]) == '!' && (formula[sta+1]) == '=') return 2;
if ((len >= 2) && (formula[sta+0]) == '&' && (formula[sta+1]) == '&') return 2;
if ((len >= 2) && (formula[sta+0]) == '|' && (formula[sta+1]) == '|') return 2;
if ((len >= 1) && (formula[sta+0]) == '*') return 1;
if ((len >= 1) && (formula[sta+0]) == 'X') return 1;
if ((len >= 1) && (formula[sta+0]) == '+') return 1;
if ((len >= 1) && (formula[sta+0]) == '-') return 1;
if ((len >= 1) && (formula[sta+0]) == '/') return 1;
if ((len >= 1) && (formula[sta+0]) == '%') return 1;
if ((len >= 1) && (formula[sta+0]) == '>') return 1;
if ((len >= 1) && (formula[sta+0]) == '<') return 1;
if ((len >= 1) && (formula[sta+0]) == '!') return 1;
return 0;
}
