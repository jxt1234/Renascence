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
    ~carryTree(){if(NULL!=mRoot) delete mRoot;}
    virtual outputType output()=0;
    virtual bool readyToOutput()=0;
    std::vector<outputType> searchAll()
    {
        std::vector<outputType> result;
        searchUnit(result, 0, 0);
        return result;
    }
    outputType searchOne(int offset=0)
    {
        std::vector<outputType> result;
        searchUnit(result, offset, 1);
        if (result.empty())
        {
            outputType def;
            return def;
        }
        return *(result.begin());
    }
protected:
    carryPoint* mRoot;
    void searchUnit(std::vector<outputType>& result, int offset, int n)
    {
        if (NULL == mRoot) return;
        int num = 0;
        while(mRoot->next())
        {
            if (readyToOutput())
            {
                if (num>=offset && (num<offset+n || n==0))
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
