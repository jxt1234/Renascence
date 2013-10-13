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
