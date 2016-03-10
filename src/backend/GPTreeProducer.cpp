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
#include <list>
#include <algorithm>
#include <sstream>
#include <stdlib.h>
#include "utils/GPDebug.h"
#include "utils/GPRandom.h"
#include "backend/GPTreeProducer.h"
#include "backend/GPTreeADF.h"
using namespace std;

class GPFunctionTreeCopy:public GPAbstractPoint::IPointCopy
{
public:
    GPFunctionTreeCopy(){}
    virtual ~GPFunctionTreeCopy(){}
    virtual GPAbstractPoint* copy(GPAbstractPoint* src, bool&)
    {
        GPFunctionTreePoint* point = (GPFunctionTreePoint*)(src);
        GPASSERT(NULL!=point);
        switch (point->type())
        {
            case GPFunctionTreePoint::FUNCTION:
                return new GPTreeADFPoint(point->data().pFunc, -1);
            case GPFunctionTreePoint::INPUT:
                return new GPTreeADFPoint(NULL, point->data().iInput);
            default:
                GPASSERT(0);
                break;
        }
        return NULL;
    }
};


IGPAutoDefFunction* GPTreeProducer::vCreateFromFuncTree(const GPFunctionTree* tree) const
{
    GPFunctionTreeCopy c;
    GPTreeADFPoint* p = (GPTreeADFPoint*)(GPAbstractPoint::deepCopy(tree->root(), &c));
    return new GPTreeADF(p);
}


GPTreeProducer::GPTreeProducer()
{
}

IGPAutoDefFunction* GPTreeProducer::vCreateFromNode(const GPTreeNode* node, const GPFunctionDataBase* base) const
{
    GPASSERT(node->name() == "GPTreeADF");
    GPTreeADFPoint* point = GPTreeADFPoint::xmlLoad(node->getChildren()[0].get(), base);
    return new GPTreeADF(point);
}


