#include "midend/GPMultiLayerTree.h"
#include <set>
GPMultiLayerTree::GPMultiLayerTree(const GPFunctionTree* tree)
{
    GPPtr<GPFunctionTreePoint> root = GPFunctionTree::copy(tree->root());
    {
        POINTS firstLayer;
        firstLayer.insert(std::make_pair(-1, root));
        mLayers.push_back(firstLayer);
    }
    /*Find Origin Inputs*/
    int maxInputNumber = -1;
    for (auto p : root->display())
    {
        auto pp = GPCONVERT(const GPFunctionTreePoint, p);
        if (pp->inputNumber()>maxInputNumber)
        {
            maxInputNumber = pp->inputNumber();
        }
    }
    int subtree_inputpos = maxInputNumber+1;
    int currentLayer = 0;
    std::vector<GPFunctionTreePoint*> workLists;
    do
    {
        /*Generate Next workLists*/
        workLists.clear();
        for (int cur=0; cur<=currentLayer; ++cur)
        {
            for (auto iter :mLayers[cur])
            {
                auto childrens = iter.second->display();
                for (auto p : childrens)
                {
                    auto pp = GPCONVERT(const GPFunctionTreePoint, p);
                    if (pp->function()!=NULL)
                    {
                        workLists.push_back((GPFunctionTreePoint*)pp);
                    }
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
        
        POINTS layer;
        /*Generate Layer*/
        for (auto iter : equallist)
        {
            GPFunctionTreePoint* inputpoints = NULL;
            /*Find iter.first in previous layer*/
            for (int layeriter=1; layeriter<mLayers.size(); ++layeriter)
            {
                for (auto l : mLayers[layeriter])
                {
                    if (iter.first->equal(l.second.get()))
                    {
                        inputpoints = new GPFunctionTreePoint(NULL, l.first);
                        break;
                    }
                }
            }
            /*Not find exists tree, create a new one*/
            if (NULL == inputpoints)
            {
                /*replace will decRef the iter.first, addRef to avoid free*/
                iter.first->addRef();
                inputpoints = new GPFunctionTreePoint(NULL, subtree_inputpos);
                GPPtr<GPFunctionTreePoint> match = (GPFunctionTreePoint*)iter.first;
                layer.insert(std::make_pair(subtree_inputpos, match));
                subtree_inputpos++;
            }
            for (int cur=0; cur<=currentLayer; ++cur)
            {
                for (auto root_iter : mLayers[cur])
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
        if (!layer.empty())
        {
            mLayers.push_back(layer);
            currentLayer++;
        }
        if (equallist.empty())
        {
            break;
        }
    }while (!workLists.empty());

}

GPMultiLayerTree::~GPMultiLayerTree()
{
    
}
