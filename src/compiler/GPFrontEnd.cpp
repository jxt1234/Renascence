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
#include "GPFrontEnd.h"
#include "pystring.h"
#include "utils/GPDebug.h"
#include <string.h>

static GP__Point* _createFormula(const std::string& formula)
{
    GPASSERT(!formula.empty());
    auto mid_start = formula.find("(");
    auto mid_end = formula.rfind(")");
    if (mid_end == std::string::npos && mid_start == std::string::npos)
    {
        //Variable
        GP__Point* point = (GP__Point*)::malloc(sizeof(GP__Point));
        gp__point__init(point);
        point->type = GP__POINT__TYPE__INPUT;
        point->content = ::strdup(formula.c_str());
        return point;
    }
    else if (mid_start != std::string::npos && mid_end != std::string::npos)
    {
        //Function
        GP__Point* point = (GP__Point*)::malloc(sizeof(GP__Point));
        gp__point__init(point);
        point->type = GP__POINT__TYPE__FUNCTION;
        point->content = ::strdup(formula.substr(0, mid_start).c_str());
        auto variable_string = formula.substr(mid_start+1, mid_end-mid_start-1);
        std::vector<std::string> variables;
        pystring::split(variable_string, variables, ",");
        point->n_input_variable = variables.size();
        point->input_variable = (GP__Point**)malloc(sizeof(GP__Point*)*variables.size());
        for (int i=0; i<variables.size(); ++i)
        {
            point->input_variable[i] = _createFormula(variables[i]);
        }
        return point;
    }
    GPPRINT_FL("Invalid subpart: %s", formula.c_str());
    return NULL;
}

static GP__Point* _create(const std::string& sentence)
{
    if (sentence.find("=") == std::string::npos)
    {
        GPPRINT_FL("%s", "Has No =");
        return NULL;
    }
    //Create Output
    std::vector<std::string> parts;
    pystring::split(sentence, parts, "=");
    if (parts.size() != 2)
    {
        GPPRINT_FL("%s", "Has more than one = ");
        return NULL;
    }
    auto output = parts[0];
    output = pystring::replace(output, "(", "");
    output = pystring::replace(output, ")", "");
    GP__Point* output_p = (GP__Point*)::malloc(sizeof(GP__Point));
    gp__point__init(output_p);
    {
        std::vector<std::string> variables;
        pystring::split(output, variables, ",");
        output_p->n_output_names = variables.size();
        output_p->output_names = (char**)::malloc(sizeof(char*)*variables.size());
        for (int i=0; i<variables.size(); ++i)
        {
            output_p->output_names[i] = ::strdup(variables[i].c_str());
        }
    }
    output_p->n_input_variable = 1;
    output_p->type = GP__POINT__TYPE__OUTPUT;
    output_p->input_variable = (GP__Point**)::malloc(sizeof(GP__Point*));
    output_p->input_variable[0] = _createFormula(parts[1]);
    return output_p;
}

GP__PointGroup* GPFrontEnd::vCreate(const char* content, const char** errorInfo) const
{
    GPASSERT(NULL!=content);
    std::string program = content;
    int stapos = 0;
    for (;program[stapos]!='{' && stapos<program.size();stapos++);
    if (stapos >= program.size()-1)
    {
        GPPRINT_FL("%s", "Invalid program without { ");
        return NULL;
    }
    int endPos = (int)program.size()-1;
    for (;program[endPos]!='}' && endPos>=0;endPos--);
    if (endPos <= 0)
    {
        GPPRINT_FL("%s", "Invalid program without }");
        return NULL;
    }
    program = program.substr(stapos+1, endPos-stapos-1);
    
    program = pystring::replace(program, " ", "");
    program = pystring::replace(program, "\n", "");
    program = pystring::replace(program, "\t", "");
    program = pystring::replace(program, "{", "(");
    program = pystring::replace(program, "[", "(");
    program = pystring::replace(program, "}", ")");
    program = pystring::replace(program, "]", ")");
    std::vector<std::string> sentences;
    pystring::split(program, sentences, ";");
    std::vector<GP__Point*> points;
    
    std::vector<std::string> errors;
    auto errorFunction = [&errors](const std::string& error, const std::string& sentence){
        auto errorString = sentence + " : " + error;
        FUNC_PRINT_ALL(errorString.c_str(), s);
    };
    
    for (auto& sentence : sentences)
    {
        if (sentence.size() <= 1)
        {
            continue;
        }
        auto p = _create(sentence);
        if (NULL==p)
        {
            errorFunction("Invalid sentence", sentence);
            continue;
        }
        points.push_back(p);
    }
    if (points.empty())
    {
        return NULL;
    }
    
    GP__PointGroup* result = (GP__PointGroup*)::malloc(sizeof(GP__PointGroup));
    gp__point_group__init(result);
    result->n_formulas = points.size();
    result->formulas = (GP__Point**)::malloc(sizeof(GP__Point*)*points.size());
    ::memcpy(result->formulas, points.data(), sizeof(GP__Point*)*points.size());
    
    
    return result;
}
