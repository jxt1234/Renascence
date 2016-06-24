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

GPFunctionTree* GPMapReduceMerger::reduce(const GPFunctionTree* tree)
{
    GPASSERT(NULL!=tree);
    auto copyRoot = _reduceCopy(tree->root());
    GPASSERT(NULL!=copyRoot);
    return new GPFunctionTree(copyRoot);
}

static bool _canMerge(const GPFunctionTreePoint* point)
{
    auto input = (GPFunctionTreePoint*)point->getChild(0);
    if (IParallelMachine::REDUCE == point->data().iParallelType)
    {
        return false;
    }
    bool hasChildMap = false;
    for (int i=0; i<input->getChildrenNumber(); ++i)
    {
        auto subInput = (GPFunctionTreePoint*)input->getChild(i);
        if (GPFunctionTreePoint::PARALLEL == subInput->type() && IParallelMachine::MAP == point->data().iParallelType)
        {
            hasChildMap = true;
        }
    }
    return hasChildMap;
}

GPFunctionTreePoint* GPMapReduceMerger::_reduceCopy(const GPFunctionTreePoint* point)
{
    if(point->type() != GPFunctionTreePoint::PARALLEL)
    {
        return GPFunctionTree::copy(point);
    }
    /*0: Input, 1: func, 2: keymap, 3: condition*/
    GPASSERT(point->getChildrenNumber()>=3);
    bool canMerge = _canMerge(point);
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
    int currentLastIndex = input->getChildrenNumber();
    bool complete = true;
    for (int i=0; i<input->getChildrenNumber(); ++i)
    {
        GPFunctionTreePoint* subInput = (GPFunctionTreePoint*)input->getChild(i);
        std::ostringstream os;
        os << "x" << i;
        std::string x = os.str();
        if (subInput->type() == GPFunctionTreePoint::PARALLEL)
        {
            auto subInputKeyMap = (GPFunctionTreePoint*)subInput->getChild(2);
            auto subInputKeyMapOutput =(GPFunctionTreePoint*)subInputKeyMap->getChild(1);
            if (subInput->data().iParallelType == IParallelMachine::MAP && subInputKeyMapOutput->getChildrenNumber() == 1)
            {
                //Replace the inputs
                
                
                //Merge to the function formula
                //Firstly, replace x1,x2... by xn+1, xn+2, in the sub formula
                //Secondly, replace xi by sub formula
                
                
                //Merge the keymap
                
                
                //Merge the condition
                
                continue;
            }
        }
    }
    return root;
}
