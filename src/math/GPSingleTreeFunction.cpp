#include "math/GPSingleTreeFunction.h"
#include <math.h>
int GPSingleTreeFunction::func(const std::string& name)
{
if (name == "*") return 0;
if (name == "X") return 1;
if (name == "ln") return 2;
if (name == "+") return 3;
if (name == "-") return 4;
if (name == "/") return 5;
if (name == ">") return 6;
if (name == "<") return 7;
if (name == ">=") return 8;
if (name == "<=") return 9;
if (name == "==") return 10;
if (name == "!=") return 11;
if (name == "sin") return 12;
if (name == "cos") return 13;
if (name == "sqrt") return 14;
return -1;
}
std::string GPSingleTreeFunction::query(int func)
{
if (func == 0) return "*";
if (func == 1) return "X";
if (func == 2) return "ln";
if (func == 3) return "+";
if (func == 4) return "-";
if (func == 5) return "/";
if (func == 6) return ">";
if (func == 7) return "<";
if (func == 8) return ">=";
if (func == 9) return "<=";
if (func == 10) return "==";
if (func == 11) return "!=";
if (func == 12) return "sin";
if (func == 13) return "cos";
if (func == 14) return "sqrt";
return "";
}
int GPSingleTreeFunction::inputNumber(int func)
{
if (func == 0) return 2;
if (func == 1) return 2;
if (func == 2) return 1;
if (func == 3) return 2;
if (func == 4) return 2;
if (func == 5) return 2;
if (func == 6) return 2;
if (func == 7) return 2;
if (func == 8) return 2;
if (func == 9) return 2;
if (func == 10) return 2;
if (func == 11) return 2;
if (func == 12) return 1;
if (func == 13) return 1;
if (func == 14) return 1;
return -1;
}
GPFLOAT GPSingleTreeFunction::compute(int func, GPFLOAT* x)
{
if (func == 0) return (x[0]*x[1]);
if (func == 1) return (x[0]*x[1]);
if (func == 2) return log(x[0]);
if (func == 3) return (x[0]+x[1]);
if (func == 4) return (x[0]-x[1]);
if (func == 5) return (x[0]/x[1]);
if (func == 6) return (x[0]>x[1]?1:0);
if (func == 7) return (x[0]<x[1]?1:0);
if (func == 8) return (x[0]>=x[1]?1:0);
if (func == 9) return (x[0]<=x[1]?1:0);
if (func == 10) return (x[0]==x[1]?1:0);
if (func == 11) return (x[0]!=x[1]?1:0);
if (func == 12) return sin(x[0]);
if (func == 13) return cos(x[0]);
if (func == 14) return sqrt(x[0]);
return 0;
}
int GPSingleTreeFunction::priority(int func)
{
if (func == 0) return 5;
if (func == 1) return 5;
if (func == 2) return 7;
if (func == 3) return 3;
if (func == 4) return 3;
if (func == 5) return 5;
if (func == 6) return 1;
if (func == 7) return 1;
if (func == 8) return 1;
if (func == 9) return 1;
if (func == 10) return 1;
if (func == 11) return 1;
if (func == 12) return 7;
if (func == 13) return 7;
if (func == 14) return 7;
return 0;
}
