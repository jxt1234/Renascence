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
#ifndef BASIC_TREE_H
#define BASIC_TREE_H

#include <vector>
#include <list>

template<class T>
class BasicTree
{
    public:
        BasicTree(){mRoot = NULL;}
        void searchAndDo(void(*func)(T*))
        {
            if(NULL!=mRoot)
            {
                searchAndDoUnit(mRoot, func);
            }
        }
        virtual ~BasicTree()
        {
            if (NULL!=mRoot)
            {
                deleteUnit(mRoot);
            }
        }
    protected:
        void searchAndDoUnit(T* point, void(*func)(T*))
        {
            for(int i=0; i<point->childern.size(); ++i)
            {
                searchAndDoUnit(point->childern[i], func);
            }
            func(point);
        }
        void deleteUnit(T* point)
        {
            for(int i=0; i<point->childern.size(); ++i)
            {
                deleteUnit(point->childern[i]);
            }
            delete point;
        }
        T* mRoot;
};
#endif
