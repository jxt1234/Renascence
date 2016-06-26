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
#include "frontend/GPFormulaTree.h"
#include "midend/GPMapReduceMerger.h"
#include "math/GPSingleTree.h"
#include <queue>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <list>

GPFunctionTree* GPMapReduceMerger::reduce(const GPFunctionTree* tree)
{
    GPASSERT(NULL!=tree);
    auto copyRoot = _reduceCopy(tree->root());
    GPASSERT(NULL!=copyRoot);
    return new GPFunctionTree(copyRoot);
}

void GPMapReduceMerger::_reduceInput(GPFunctionTreePoint* root)
{
    GPASSERT(NULL!=root);
    /*0: Input, 1: func, 2: keymap, 3: condition*/
    GPASSERT(root->getChildrenNumber() == 4);
    GPASSERT(GPFunctionTreePoint::PARALLEL == root->type());
    auto input = GPFORCECONVERT(GPFunctionTreePoint, root->getChild(0));
    auto originInputNumber = input->getChildrenNumber();
    std::set<int> inputSet;
    std::map<int, std::string> inputReplaceMap;
    for (int i=0; i<originInputNumber; ++i)
    {
        auto subInput = GPFORCECONVERT(GPFunctionTreePoint, input->getChild(i));
        if (subInput->type() != GPFunctionTreePoint::PARALLEL)
        {
            inputSet.insert(subInput->data().iInput);
        }
    }
    
    
    auto funcPoint = GPFORCECONVERT(GPFunctionTreePoint, root->getChild(1));
    std::string formula = GPCONVERT(const GPFunctionTreePoint, funcPoint->getChild(1))->extra();
    std::string formulaVariableInfo = GPCONVERT(const GPFunctionTreePoint, funcPoint->getChild(0))->extra();
    auto keyMapPoint = GPFORCECONVERT(GPFunctionTreePoint, root->getChild(2));
    auto keyMapInputPoint = GPFORCECONVERT(GPFunctionTreePoint, keyMapPoint->getChild(0));

}




GPFunctionTreePoint* GPMapReduceMerger::_reduceCopy(const GPFunctionTreePoint* point)
{
    if(point->type() != GPFunctionTreePoint::PARALLEL)
    {
        return GPFunctionTree::copy(point);
    }
    /*0: Input, 1: func, 2: keymap, 3: condition*/
    GPASSERT(point->getChildrenNumber() == 4);
    auto input = GPFORCECONVERT(GPFunctionTreePoint, point->getChild(0));
    GPFunctionTreePoint* root = new GPFunctionTreePoint(*point);
    GPFunctionTreePoint* copyInput = new GPFunctionTreePoint(*input);
    root->addPoint(copyInput);
    /*Copy Input*/
    for (int i=0; i<input->getChildrenNumber(); ++i)
    {
        auto subInput = (GPFunctionTreePoint*)input->getChild(i);
        copyInput->addPoint(_reduceCopy(subInput));
    }
    /*Copy Others*/
    for (int i=1; i<point->getChildrenNumber(); ++i)
    {
        root->addPoint(GPFunctionTree::copy((GPFunctionTreePoint*)point->getChild(i)));
    }

    if (root->data().iParallelType != IParallelMachine::MAP)
    {
        return root;
    }

    /*Start Merge*/
    input = GPFORCECONVERT(GPFunctionTreePoint, root->getChild(0));
    auto conditionPoint = GPFORCECONVERT(GPFunctionTreePoint, root->getChild(3));
    std::string condition = conditionPoint->extra();

    auto keyMapPoint = GPFORCECONVERT(GPFunctionTreePoint, root->getChild(2));
    auto keyMapInputPoint = GPFORCECONVERT(GPFunctionTreePoint, keyMapPoint->getChild(0));

    auto funcPoint = GPFORCECONVERT(GPFunctionTreePoint, root->getChild(1));
    std::string formula = GPCONVERT(const GPFunctionTreePoint, funcPoint->getChild(1))->extra();
    std::string formulaVariableInfo = GPCONVERT(const GPFunctionTreePoint, funcPoint->getChild(0))->extra();
    std::ostringstream formulaVariableInfoOs;
    formulaVariableInfoOs << formulaVariableInfo;

    /*Generate Replace Map by formulaVariableInfo*/
    std::map<std::string, std::string> replaceVariableMap;
    {
        /*For Map is useless*/
        int pos = 0;
        std::istringstream inputString(formulaVariableInfo);
        std::string forReplace;
        while(inputString >> forReplace)
        {
            std::ostringstream os;
            os << "x" <<pos++;
            replaceVariableMap.insert(std::make_pair(os.str(), forReplace));
        }
    }

    int initNumber = input->getChildrenNumber();
    std::map<std::string, std::string> formulaReplaceMap;
    for (int i=0; i<initNumber; ++i)
    {
        GPFunctionTreePoint* subInput = (GPFunctionTreePoint*)input->getChild(i);
        if (subInput->type() == GPFunctionTreePoint::PARALLEL)
        {
            auto subInputKeyMap = (GPFunctionTreePoint*)subInput->getChild(2);
            auto subInputInput =(GPFunctionTreePoint*)subInput->getChild(0);
            auto subInputKeyMapOutput =(GPFunctionTreePoint*)subInputKeyMap->getChild(1);
            auto subInputCondition = (GPFunctionTreePoint*)subInput->getChild(3);
            if (subInput->data().iParallelType == IParallelMachine::MAP && subInputKeyMapOutput->getChildrenNumber() == 1 && subInputCondition->extra().empty())
            {
                int startNumber = input->getChildrenNumber()-1;
                //Replace the inputs
                for (int j=1; j<subInputInput->getChildrenNumber(); ++j)
                {
                    auto subpp = GPFORCECONVERT(GPFunctionTreePoint, subInputInput->getChild(j));
                    subpp->addRef();
                    input->addPoint(subpp);
                }
                
                //Merge to the function formula
                //Firstly, replace x1,x2... by xn+1, xn+2, in the sub formula
                auto subFuncPoint = subInput->getChild(1);
                std::string subFormula = GPCONVERT(const GPFunctionTreePoint, subFuncPoint->getChild(1))->extra();
                std::map<std::string, std::string> replaceConditionMap;
                std::map<std::string, std::string> subFormulaReplaceMap;
                {
                    std::ostringstream os;
                    os << "x" << i;
                    auto map_before = os.str();
                    std::string x_before = replaceVariableMap.find(map_before)->second;
                    subFormulaReplaceMap.insert(std::make_pair("x0", x_before));
                }
                GPFormulaTree subFormulaTree;
                subFormulaTree.setFormula(subFormula);
//                FUNC_PRINT_ALL(subFormula.c_str(), s);
//                FUNC_PRINT(subInputInput->getChildrenNumber());
                for (int j=1; j<subInputInput->getChildrenNumber(); ++j)
                {
                    std::ostringstream os;
                    os << "x" << j+startNumber;
                    std::string x_after = os.str();
                    formulaVariableInfoOs << " "<<x_after;
                    os.str("");
                    char c = 'a'+j;
                    os << c <<"0";
                    auto cv_before = os.str();
                    os.str("");
                    c = ('a'+j+startNumber);
                    os << c << "0";
                    auto cv_after = os.str();

                    /*Merge the keymap input*/
                    keyMapInputPoint->addPoint(new GPFunctionTreePoint(cv_after));
                    replaceConditionMap.insert(std::make_pair(cv_before, cv_after));
                }
                for (int j=subInputInput->getChildrenNumber()-1; j>0; --j)
                {
                    std::ostringstream os;
                    os << "x" << j;
                    std::string x_before = os.str();
                    os.str("");
                    os << "x" << j+startNumber;
                    std::string x_after = os.str();
                    subFormulaReplaceMap.insert(std::make_pair(x_before, x_after));
                }
                subFormulaTree.root()->replaceNameAll(subFormulaReplaceMap);
                subFormula = subFormulaTree.dump();
//                FUNC_PRINT_ALL(subFormula.c_str(), s);
                //Secondly, replace xi by sub formula
                {
                    std::ostringstream os;
                    os << "x" << i;
                    auto map_before = os.str();
                    std::string x_before = replaceVariableMap.find(map_before)->second;
                    GPASSERT(map_before == x_before);
                    formulaReplaceMap.insert(std::make_pair(x_before, subFormula));
                }

                //Replace condition's key
                auto subCondition = GPCONVERT(const GPFunctionTreePoint, subInput->getChild(3))->extra();
                if (!subCondition.empty())
                {
                    auto words = GPSingleTree::divideWords(subCondition);
                    for (int pos = 0; pos < words.size(); ++pos)
                    {
                        if (replaceConditionMap.find(words[pos])!=replaceConditionMap.end())
                        {
                            words[pos] = replaceConditionMap.find(words[pos])->second;
                        }
                    }
                    std::ostringstream subConditionOs;
                    for (int pos = 0; pos < words.size(); ++pos)
                    {
                        subConditionOs << words[pos];
                    }
                    subCondition = subConditionOs.str();
                    if (condition.empty())
                    {
                        condition = subCondition;
                    }
                    else
                    {
                        std::ostringstream oscondition;
                        oscondition << condition << " && (" << subCondition << ")";
                        condition = oscondition.str();
                    }
                }
                /*At last replace the input self*/
                auto firstP = GPFORCECONVERT(GPFunctionTreePoint, subInputInput->getChild(0));
                input->replace(subInput, firstP);
            }
        }
    }
    
    GPFormulaTree formulaTree;
    formulaTree.setFormula(formula);
    formulaTree.root()->replaceNameAll(formulaReplaceMap);
    formula = formulaTree.dump();

    /*Update FunctionPoint*/
    auto variablePoint = new GPFunctionTreePoint(formulaVariableInfoOs.str());
    funcPoint->replace(GPFORCECONVERT(GPFunctionTreePoint,funcPoint->getChild(0)), variablePoint);
    variablePoint->decRef();
    auto formulaPoint = new GPFunctionTreePoint(formula);
    funcPoint->replace(GPFORCECONVERT(GPFunctionTreePoint,funcPoint->getChild(1)), formulaPoint);
    formulaPoint->decRef();

    /*Update Condition*/
    auto newCondition = new GPFunctionTreePoint(condition);
    root->replace(conditionPoint, newCondition);
    newCondition->decRef();
    return root;
}
