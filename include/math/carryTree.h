/******************************************************************
   Copyright 2013, Jiang Xiao-tang

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
#ifndef CARRY_TREE_H
#define CARRY_TREE_H
#include "carryPoint.h"
template <class outputType>
class carryTree
{
public:
    carryTree(carryPoint* root=NULL):mRoot(root){}
    ~carryTree(){}
    virtual outputType output()=0;
    virtual bool readyToOutput()=0;
    std::vector<outputType> searchAll()
    {
        std::vector<outputType> result;
        searchUnit(result, 0);
        return result;
    }
    bool searchOne(outputType* dst)
    {
        if (NULL == mRoot.get()) return false;
        while(mRoot->next())
        {
            if (readyToOutput())
            {
                *dst = output();
                return true;
            }
        }
        return false;
    }
protected:
    GPPtr<carryPoint> mRoot;
    void searchUnit(std::vector<outputType>& result, int n)
    {
        if (NULL == mRoot.get()) return;
        int num = 0;
        while(mRoot->next())
        {
            if (readyToOutput())
            {
                if (num<n || n==0)
                {
                    result.push_back(output());
                    num++;
                }
                else
                {
                    break;
                }
            }
        }
    }
};



#endif
