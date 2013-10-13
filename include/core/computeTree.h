#ifndef COMPUTE_TREE_H
#define COMPUTE_TREE_H
#include "utils/BasicTree.h"
#include "function.h"
struct computePoint
{
    bool saved;
    vector<void*> output;
    computeFunction func;
    vector<void*> status;
    vector<computePoint*> childern;
};

class computeTree:public BasicTree<computePoint>
{
    public:
        computeTree(){}
        virtual ~computeTree(){}
};

#endif
