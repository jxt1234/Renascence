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
#include "frontend/GPFormulaTree.h"
template<typename T>
static T* _vector2Array(const std::vector<T>& src)
{
    if (src.empty())
    {
        return NULL;
    }
    T* dst = (T*)malloc(sizeof(T)*src.size());
    ::memcpy(dst, src.data(), sizeof(T)*src.size());
    
    return dst;
}

static GP__Point* _createPoint(const GPFormulaTreePoint* point)
{
    GP__Point* p = NULL;
    p = (GP__Point*)::malloc(sizeof(GP__Point));
    gp__point__init(p);
    p->content = ::strdup(point->name().c_str());
    switch (point->type())
    {
        case GPFormulaTreePoint::NUM:
            p->type = GP__POINT__TYPE__INPUT;
            break;
        case GPFormulaTreePoint::OPERATOR:
            p->type = GP__POINT__TYPE__FUNCTION;
            break;
        default:
            break;
    }
    p->n_input_variable = point->getChildrenNumber();
    p->input_variable = (GP__Point**)::malloc(sizeof(GP__Point*)*point->getChildrenNumber());
    for (int i=0; i<point->getChildrenNumber(); ++i)
    {
        GPFormulaTreePoint* subp = GPFORCECONVERT(GPFormulaTreePoint, point->getChild(i));
        p->input_variable[i] = _createPoint(subp);
    }
    return p;
}


static GP__Point* _createFormula(const std::string& formula)
{
    GPASSERT(!formula.empty());
    GPFormulaTree formulaTree;
    formulaTree.setFormula(formula);
    return _createPoint(formulaTree.root());
}

static GP__ADF* _createADF(const std::string& sentence, std::function<bool(const std::string& error, const std::string sentence)> errorFunction)
{
    std::vector<std::string> parts;
    pystring::split(sentence, parts, "=");
    if (parts.size() != 2)
    {
        GPPRINT_FL("%s", "Has more than one = ");
        return NULL;
    }
    auto outputVariable = parts[0];
    auto adfFormula = parts[1];
    GPFormulaTree formulaTree;
    formulaTree.setFormula(adfFormula);
    auto root = formulaTree.root();
    GPASSERT(root->name() == "ADF");
    GPASSERT(root->getChildrenNumber() == 2);
    GP__ADF* adf_result = (GP__ADF*)malloc(sizeof(GP__ADF));
    gp__adf__init(adf_result);
    adf_result->name = ::strdup(outputVariable.c_str());
    auto inputPoint = GPFORCECONVERT(GPFormulaTreePoint, root->getChild(1));
    GPASSERT(inputPoint->name() == "Input");
    auto outputPoint = GPFORCECONVERT(GPFormulaTreePoint, root->getChild(0));
    GPASSERT(outputPoint->name() == "Output");
    adf_result->n_input_types  = inputPoint->getChildrenNumber();
    adf_result->input_types = (char**)malloc(sizeof(char*)*adf_result->n_input_types);
    for (int i=0; i<adf_result->n_input_types; ++i)
    {
        adf_result->input_types[i] = ::strdup(GPFORCECONVERT(GPFormulaTreePoint, inputPoint->getChild(i))->name().c_str());
    }
    adf_result->n_output_types = outputPoint->getChildrenNumber();
    adf_result->output_types = (char**)malloc(sizeof(char*)*outputPoint->getChildrenNumber());
    for (int i=0; i<outputPoint->getChildrenNumber(); ++i)
    {
        adf_result->output_types[i] = ::strdup(GPFORCECONVERT(GPFormulaTreePoint, outputPoint->getChild(i))->name().c_str());
    }
    return adf_result;
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
    output_p->type = GP__POINT__TYPE__OUTPUT;
    output_p->n_input_variable = 1;
    output_p->input_variable = (GP__Point**)::malloc(sizeof(GP__Point*));
    output_p->input_variable[0] = _createFormula(parts[1]);
    return output_p;
}

GP__PointGroup* GPFrontEnd::vCreate(const char* content, char** errorInfo) const
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
    std::vector<GP__ADF*> adfs;

    
    std::vector<std::string> errors;
    auto errorFunction = [&errors](const std::string& error, const std::string& sentence){
        auto errorString = sentence + " : " + error;
        FUNC_PRINT_ALL(errorString.c_str(), s);
        return true;
    };
    
    for (auto& sentence : sentences)
    {
        if (sentence.size() <= 1)
        {
            continue;
        }
        if (sentence.find("ADF")!=std::string::npos)
        {
            auto adf = _createADF(sentence, errorFunction);
            if (NULL!=adf)
            {
                adfs.push_back(adf);
            }
            else
            {
                errorFunction("Invalid sentence", sentence);
            }
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
        GPASSERT(false);//TODO
        return NULL;
    }

    GP__PointGroup* result = (GP__PointGroup*)::malloc(sizeof(GP__PointGroup));
    gp__point_group__init(result);
    result->n_formulas = points.size();
    result->formulas = _vector2Array(points);
    result->n_adfs = adfs.size();
    result->adfs = _vector2Array(adfs);
    
    return result;
}
