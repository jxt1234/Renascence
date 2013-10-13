#ifndef VECTOR_POINT_TREE_H
#define VECTOR_POINT_TREE_H
#include "math/carryTree.h"
#include "system/computeSystem.h"
#include "utils/debug.h"
class computePoint:public carryPoint
{
friend class computeSearchTree;
public:
    computePoint(const vector<vector<int> >& data, const vector<int>& avail):mData(data), mAvail(avail), mCur(0){}
    const vector<int>& getData() const{return mData[mAvail[mCur]];}
    static vector<int> filter(const vector<vector<int> >& combo, vector<int> output);
    static void setComputeSystem(computeSystem* sys){gSystem = sys;}
protected:
    virtual bool vGrow();
    vector<int> getDependOutput();
    virtual bool vNext(){mCur++;return mCur<mAvail.size();}
    static computeSystem* gSystem;
private:
    const vector<vector<int> >& mData;
    vector<int> mAvail;
    int mCur;
    int mParent;
};


class computeSearchTree:public carryTree<vector<int> >
{
public:
    computeSearchTree(computePoint* point){mRoot = point;}
    virtual vector<int> output();
    virtual bool readyToOutput();
};

#endif
