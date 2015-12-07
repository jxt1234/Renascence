/******************************************************************
 Copyright 2015, Jiang Xiao-tang
 
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

#include "backend/GPStreamADFProducer.h"
#include "midend/GPMultiLayerTree.h"
#include "GPStreamADF.h"
#include <iostream>

GPStreamADFProducer::GPStreamADFProducer()
{
}
GPStreamADFProducer::~GPStreamADFProducer()
{
}
IGPAutoDefFunction* GPStreamADFProducer::vCreateFromFuncTree(const GPFunctionTree* tree) const
{
    /*TODO Add Optional to Disable Optimize*/
    //return new GPStreamADF(tree);
    GPASSERT(NULL!=tree);
    //std::cout << tree->dump() << "\n";
    GPPtr<GPMultiLayerTree> multi_tree = new GPMultiLayerTree(tree);
#if 0
    for (auto l : multi_tree->layers())
    {
        for (auto iter:l)
        {
            std::cout << iter.first << ": ";
            iter.second->render(std::cout);
            std::cout << "\n";
        }
    }
#endif
    return new GPStreamADF(multi_tree.get());
}
IGPAutoDefFunction* GPStreamADFProducer::vCreateFromNode(const GPTreeNode* p, const GPFunctionDataBase* base) const
{
    GPASSERT(NULL!=p);
    if (p->name() != "GPStreamADF")
    {
        return NULL;
    }
    return new GPStreamADF(p, base);
}


