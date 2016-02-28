#include "midend/GPMultiLayerTree.h"
#include <set>
GPMultiLayerTree::GPMultiLayerTree(const GPFunctionTree* tree)
{
    GPPtr<GPFunctionTreePoint> root = GPFunctionTree::copy(tree->root());
    mLayers.insert(std::make_pair(-1, root));
    /*Find Origin Inputs*/
    int maxInputNumber = -1;
    for (auto p : root->display())
    {
        auto pp = GPCONVERT(const GPFunctionTreePoint, p);
        if (pp->data().iInput>maxInputNumber && pp->type() == GPFunctionTreePoint::INPUT)
        {
            maxInputNumber = pp->data().iInput;
        }
    }
    int subtree_inputpos = maxInputNumber+1;
    std::vector<GPFunctionTreePoint*> workLists;
    while (true)
    {
        /*Generate Next workLists*/
        workLists.clear();
        for (auto iter :mLayers)
        {
            auto childrens = iter.second->display();
            for (auto p : childrens)
            {
                auto pp = GPCONVERT(const GPFunctionTreePoint, p);
                if (GPFunctionTreePoint::FUNCTION == pp->type())
                {
                    workLists.push_back((GPFunctionTreePoint*)pp);
                }
            }
        }
        
        /*Find same subtree*/
        std::map<GPFunctionTreePoint*, std::vector<GPFunctionTreePoint*>> equallist;
        for (size_t i=0; i<workLists.size(); ++i)
        {
            auto forcompare = workLists[i];
            bool valid = true;
            for (auto iter : equallist)
            {
                if (forcompare->isChildOf(iter.first))
                {
                    valid = false;
                }
                for (auto p : iter.second)
                {
                    if (forcompare->isChildOf(p))
                    {
                        valid = false;
                        break;
                    }
                }
                if (!valid)
                {
                    break;
                }
            }
            if (!valid)
            {
                continue;
            }
            std::vector<GPFunctionTreePoint*> equalpairs;
            for (size_t j=i+1; j<workLists.size(); ++j)
            {
                if (workLists[j]->equal(forcompare))
                {
                    equalpairs.push_back(workLists[j]);
                }
            }
            /*Valid Point, Added to equallist*/
            if (!equalpairs.empty())
            {
                std::map<GPFunctionTreePoint*, std::vector<GPFunctionTreePoint*>> filtered_equallist;
                for (auto iter : equallist)
                {
                    bool valid = true;
                    if (iter.first->isChildOf(forcompare))
                    {
                        continue;
                    }
                    for (auto p : iter.second)
                    {
                        if (p->isChildOf(forcompare))
                        {
                            valid = false;
                            break;
                        }
                    }
                    if (valid)
                    {
                        filtered_equallist.insert(std::make_pair(iter.first, iter.second));
                    }
                }
                equallist = filtered_equallist;
                equallist.insert(std::make_pair(forcompare, equalpairs));
            }
        }
        
        /*Generate New Points*/
        for (auto iter : equallist)
        {
            GPFunctionTreePoint* inputpoints = NULL;
            int inputCur = -1;
            /*Find iter.first in previous layer*/
            for (auto l : mLayers)
            {
                if (iter.first->equal(l.second.get()))
                {
                    inputpoints = new GPFunctionTreePoint(l.first);
                    inputCur = l.first;
                    break;
                }
            }
            /*Not find exists tree, create a new one*/
            if (NULL == inputpoints)
            {
                /*replace will decRef the iter.first, addRef to avoid free*/
                iter.first->addRef();
                inputpoints = new GPFunctionTreePoint(subtree_inputpos);
                GPPtr<GPFunctionTreePoint> match = (GPFunctionTreePoint*)iter.first;
                mLayers.insert(std::make_pair(subtree_inputpos, match));
                inputCur = subtree_inputpos;
                subtree_inputpos++;
            }
            for (auto root_iter : mLayers)
            {
                if (root_iter.first!=inputCur)
                {
                    root_iter.second->replace((GPFunctionTreePoint*)iter.first, inputpoints);
                    for (auto p : iter.second)
                    {
                        root_iter.second->replace((GPFunctionTreePoint*)p, inputpoints);
                    }
                }
            }
            
            inputpoints->decRef();
        }
        if (equallist.empty())
        {
            break;
        }
    }
}

GPMultiLayerTree::~GPMultiLayerTree()
{
    
}
