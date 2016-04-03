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
#include "math/GPSingleTreeFunction.h"
#include <math.h>
int GPSingleTreeFunction::func(const std::string& name)
{
if (name == "sqrt") return 0;
if (name == "sin") return 1;
if (name == "cos") return 2;
if (name == "ln") return 3;
if (name == ">=") return 4;
if (name == "<=") return 5;
if (name == "==") return 6;
if (name == "!=") return 7;
if (name == "&&") return 8;
if (name == "||") return 9;
if (name == "*") return 10;
if (name == "X") return 11;
if (name == "+") return 12;
if (name == "-") return 13;
if (name == "/") return 14;
if (name == ">") return 15;
if (name == "<") return 16;
if (name == "!") return 17;
return -1;
}
std::string GPSingleTreeFunction::query(int func)
{
if (func == 0) return "sqrt";
if (func == 1) return "sin";
if (func == 2) return "cos";
if (func == 3) return "ln";
if (func == 4) return ">=";
if (func == 5) return "<=";
if (func == 6) return "==";
if (func == 7) return "!=";
if (func == 8) return "&&";
if (func == 9) return "||";
if (func == 10) return "*";
if (func == 11) return "X";
if (func == 12) return "+";
if (func == 13) return "-";
if (func == 14) return "/";
if (func == 15) return ">";
if (func == 16) return "<";
if (func == 17) return "!";
return "";
}
int GPSingleTreeFunction::inputNumber(int func)
{
if (func == 0) return 1;
if (func == 1) return 1;
if (func == 2) return 1;
if (func == 3) return 1;
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
if (func == 15) return 2;
if (func == 16) return 2;
if (func == 17) return 2;
return -1;
}
GPFLOAT GPSingleTreeFunction::compute(int func, GPFLOAT* x)
{
if (func == 0) return sqrt(x[0]);
if (func == 1) return sin(x[0]);
if (func == 2) return cos(x[0]);
if (func == 3) return log(x[0]);
if (func == 4) return (x[0]>=x[1]?1:0);
if (func == 5) return (x[0]<=x[1]?1:0);
if (func == 6) return (x[0]==x[1]?1:0);
if (func == 7) return (x[0]!=x[1]?1:0);
if (func == 8) return ((x[0]>0.5&&x[1]>0.5)?1:0);
if (func == 9) return ((x[0]>0.5||x[1]>0.5)?1:0);
if (func == 10) return (x[0]*x[1]);
if (func == 11) return (x[0]*x[1]);
if (func == 12) return (x[0]+x[1]);
if (func == 13) return (x[0]-x[1]);
if (func == 14) return (x[0]/x[1]);
if (func == 15) return (x[0]>x[1]?1:0);
if (func == 16) return (x[0]<x[1]?1:0);
if (func == 17) return (x[0]<0.5?1:0);
return 0;
}
int GPSingleTreeFunction::priority(int func)
{
if (func == 0) return 7;
if (func == 1) return 7;
if (func == 2) return 7;
if (func == 3) return 7;
if (func == 4) return 2;
if (func == 5) return 2;
if (func == 6) return 2;
if (func == 7) return 2;
if (func == 8) return 1;
if (func == 9) return 1;
if (func == 10) return 5;
if (func == 11) return 5;
if (func == 12) return 3;
if (func == 13) return 3;
if (func == 14) return 5;
if (func == 15) return 2;
if (func == 16) return 2;
if (func == 17) return 1;
return 0;
}
int GPSingleTreeFunction::searchFunction(int sta, const std::string& formula)
{
int len = (int)(formula.size()) - sta;
if ((len >= 4) && (formula[sta+0]) == 's' && (formula[sta+1]) == 'q' && (formula[sta+2]) == 'r' && (formula[sta+3]) == 't') return 4;
if ((len >= 3) && (formula[sta+0]) == 's' && (formula[sta+1]) == 'i' && (formula[sta+2]) == 'n') return 3;
if ((len >= 3) && (formula[sta+0]) == 'c' && (formula[sta+1]) == 'o' && (formula[sta+2]) == 's') return 3;
if ((len >= 2) && (formula[sta+0]) == 'l' && (formula[sta+1]) == 'n') return 2;
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
if ((len >= 1) && (formula[sta+0]) == '>') return 1;
if ((len >= 1) && (formula[sta+0]) == '<') return 1;
if ((len >= 1) && (formula[sta+0]) == '!') return 1;
return 0;
}
