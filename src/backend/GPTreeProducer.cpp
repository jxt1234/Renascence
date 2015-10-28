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
#include "core/GP_XmlString.h"
#include "utils/GPRandom.h"
#include "backend/GPTreeProducer.h"
#include "backend/GPTreeADF.h"
using namespace std;

class GPFunctionTreeCopy:public GPAbstractPoint::IPointCopy
{
public:
    GPFunctionTreeCopy(){}
    virtual ~GPFunctionTreeCopy(){}
    virtual GPAbstractPoint* copy(GPAbstractPoint* src)
    {
        GPFunctionTree* point = (GPFunctionTree*)(src);
        GPASSERT(NULL!=point);
        return new GPTreeADFPoint( point->function(), point->inputNumber());
    }
};


IGPAutoDefFunction* GPTreeProducer::vCreateFromFuncTree(const GPFunctionTree* tree) const
{
    GPFunctionTreeCopy c;
    GPTreeADFPoint* p = (GPTreeADFPoint*)(GPAbstractPoint::deepCopy((GPFunctionTree*)tree, &c));
    return new GPTreeADF(p, this);
}


GPTreeProducer::GPTreeProducer()
{
}

static bool inOrder(const std::vector<size_t>& order)
{
    for (int i=0; i<order.size(); ++i)
    {
        if (i!=order[i])
        {
            return false;
        }
    }
    return true;
}

static bool makeOrder(std::vector<const IStatusType*> inputType, const std::vector<const IStatusType*>& realinputType, std::vector<size_t>& order/*output*/)
{
    bool result = true;
    order.clear();
    for (int i=0; i<realinputType.size(); ++i)
    {
        auto t = realinputType[i];
        auto pos = std::find(inputType.begin(), inputType.end(), t);
        if (pos == inputType.end())
        {
            result = false;
            break;
        }
        *pos = NULL;
        auto id = pos - inputType.begin();
        order.push_back(id);
    }
    return result;
}


IGPAutoDefFunction* GPTreeProducer::vCreateFromNode(const GPTreeNode* node, const GPFunctionDataBase* base) const
{
    return NULL;
}


