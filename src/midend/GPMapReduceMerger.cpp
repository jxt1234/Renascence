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
#include "midend/GPMapReduceMerger.h"
#include <queue>
#include <string>
#include <sstream>
#include <list>
#include "pystring.h"

GPFunctionTree* GPMapReduceMerger::reduce(const GPFunctionTree* tree)
{
    GPASSERT(NULL!=tree);
    auto copyRoot = _reduceCopy(tree->root());
    GPASSERT(NULL!=copyRoot);
    return new GPFunctionTree(copyRoot);
}


GPFunctionTreePoint* GPMapReduceMerger::_reduceCopy(const GPFunctionTreePoint* point)
{
    if(point->type() != GPFunctionTreePoint::PARALLEL || point->data().iParallelType != IParallelMachine::MAP)
    {
        return GPFunctionTree::copy(point);
    }
    /*0: Input, 1: func, 2: keymap, 3: condition*/
    GPASSERT(point->getChildrenNumber() == 4);
    auto input = (GPFunctionTreePoint*)point->getChild(0);
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
    
    /*Start Merge*/
    input = GPFORCECONVERT(GPFunctionTreePoint, root->getChild(0));
    auto conditionPoint = GPFORCECONVERT(GPFunctionTreePoint, root->getChild(3));
    std::string condition = conditionPoint->extra();
    
    auto funcPoint = GPFORCECONVERT(GPFunctionTreePoint, root->getChild(1));
    std::string formula = GPCONVERT(const GPFunctionTreePoint, funcPoint->getChild(1))->extra();
    std::string formulaVariableInfo = GPCONVERT(const GPFunctionTreePoint, funcPoint->getChild(0))->extra();
    std::ostringstream formulaVariableInfoOs;
    formulaVariableInfoOs << formulaVariableInfo;
    /*Generate Replace Map by formulaVariableInfo*/
    std::map<std::string, std::string> replaceVariableMap;
    {
        int pos = 0;
        std::istringstream inputString(formulaVariableInfo);
        std::string forReplace;
        while(inputString >> forReplace)
        {
            std::ostringstream os;
            os << "x" <<pos++;
            replaceVariableMap.insert(std::make_pair(forReplace, os.str()));
        }
    }
    
    
    int initNumber = input->getChildrenNumber();
    
    
    for (int i=0; i<initNumber; ++i)
    {
        GPFunctionTreePoint* subInput = (GPFunctionTreePoint*)input->getChild(i);
        if (subInput->type() == GPFunctionTreePoint::PARALLEL)
        {
            auto subInputKeyMap = (GPFunctionTreePoint*)subInput->getChild(2);
            auto subInputInput =(GPFunctionTreePoint*)subInput->getChild(0);
            auto subInputKeyMapInput =(GPFunctionTreePoint*)subInputKeyMap->getChild(0);
            auto subInputKeyMapOutput =(GPFunctionTreePoint*)subInputKeyMap->getChild(1);
            if (subInput->data().iParallelType == IParallelMachine::MAP && subInputKeyMapOutput->getChildrenNumber() == 1)
            {
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
                int startNumber = input->getChildrenNumber();
                std::map<std::string, std::string> replaceConditionMap;
                for (int j=1; j<subInputInput->getChildrenNumber(); ++j)
                {
                    std::ostringstream os;
                    os << "x" << j;
                    std::string x_before = os.str();
                    FUNC_PRINT_ALL(x_before.c_str(), s);
                    os.str("");
                    os << "x" << j+startNumber;
                    std::string x_after = os.str();
                    FUNC_PRINT_ALL(x_after.c_str(), s);
                    formulaVariableInfoOs << " "<<x_after;
                    subFormula = pystring::replace(subFormula, x_before, x_after);
                    
                    os.str("");
                    char c = 'a'+j;
                    os << c <<"0";
                    auto cv_before = os.str();
                    FUNC_PRINT_ALL(cv_before.c_str(), s);
                    os.str("");
                    c = ('a'+j+startNumber);
                    os << c << "0";
                    auto cv_after = os.str();
                    FUNC_PRINT_ALL(cv_after.c_str(), s);
                    replaceConditionMap.insert(std::make_pair(cv_before, cv_after));
                }
                //Secondly, replace xi by sub formula
                {
                    std::ostringstream os;
                    os << "x" << i;
                    std::string x_before = replaceVariableMap.find(os.str())->second;
                    formula = pystring::replace(formula, x_before, subFormula);
                }

                /*Merge the keymap*/
                //TODO
                
                /*Merge the condition*/
                
                auto subCondition = GPCONVERT(const GPFunctionTreePoint, subInput->getChild(3))->extra();
                //Replace condition's key
                for (auto k : replaceConditionMap)
                {
                    subCondition = pystring::replace(subCondition, k.first, k.second);
                }
                
                if (condition.empty())
                {
                    condition = subCondition;
                }
                else if (!subCondition.empty())
                {
                    std::ostringstream oscondition;
                    oscondition << condition << " && (" << subCondition << ")";
                    condition = oscondition.str();
                }
                /*At last replace the input self*/
                auto firstP = GPFORCECONVERT(GPFunctionTreePoint, subInputInput->getChild(0));
                input->replace(subInput, firstP);
            }
        }
    }
    
    /*FIXME Update input Key Map*/
    
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
