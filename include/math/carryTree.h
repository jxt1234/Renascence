#ifndef CARRY_TREE_H
#define CARRY_TREE_H
#include "math/carryPoint.h"
template <class outputType>
class carryTree
{
public:
    carryTree(carryPoint* root=NULL):mRoot(root){}
    virtual outputType output()=0;
    virtual bool readyToOutput()=0;
    vector<outputType> searchAll()
    {
        vector<outputType> result;
        searchUnit(result, 0);
        return result;
    }
    outputType searchOne()
    {
        vector<outputType> result;
        searchUnit(result, 1);
        return *(result.begin());
    }
protected:
    carryPoint* mRoot;
    void searchUnit(vector<outputType>& result, int maxNumber)
    {
        if (NULL == mRoot) return;
        int num = 0;
        while(mRoot->next())
        {
            if (readyToOutput())
            {
                result.push_back(output());
                if (num>=maxNumber && maxNumber!=0)
                {
                    break;
                }
            }
        }
    }
};



#endif
