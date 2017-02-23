/******************************************************************
   Copyright 2017, Jiang Xiao-tang

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
#include "GPDAGFunction.h"
bool GPDAGFunction::SP::vReceive(CONTENT con, const Point* source)
{
    GPASSERT(NULL==source);
    GPASSERT(NULL!=con.get());
    GPASSERT(NULL!=con->type());
    GPASSERT(NULL!=con->content());
    for (auto& o : mOutputs)
    {
        o.second->vReceive(con, this);
    }
    return true;
}

bool GPDAGFunction::CP::vReceive(CONTENT c, const Point* source)
{
    for (int i=0; i<mInputs.size(); ++i)
    {
        if (mInputs[i] == source)
        {
            if (mPoint->receive(c, i))
            {
                auto comp = mPoint->compute();
                GPASSERT(comp->size() == mOutputs.size());
                for (int j=0; j<mOutputs.size(); ++j)
                {
                    mOutputs[j].second->vReceive(comp->getContent(mOutputs[j].first), this);
                }
                delete comp;
            }
            return true;
        }
    }
    GPASSERT(0);
    return false;
}


GPDAGFunction::DP::DP():Point(1, 0)
{
    mContents = NULL;
}


bool GPDAGFunction::DP::vReceive(CONTENT c, const Point* source)
{
    GPASSERT(1 == mInputs.size());
    GPASSERT(NULL == mContents.get());
    mContents = c;
    return false;
}


void GPDAGFunction::DP::reset()
{
    mContents = NULL;
}

bool GPDAGFunction::TP::vReceive(CONTENT c, const Point* source)
{
    GPASSERT(source == mInputs[0]);
    for (auto p : mOutputs)
    {
        p.second->vReceive(c, this);
    }
    return true;
}


GPDAGFunction::GPDAGFunction(const GP__DAG* dag, const GPFunctionDataBase* database)
{
    std::map<uint32_t, GPPtr<Point>> pointmap;
    std::map<uint32_t, GPPtr<Point>> inputPoints;
    std::map<uint32_t, GPPtr<Point>> outputPoints;
    for (int i=0; i<dag->n_points; ++i)
    {
        auto p = dag->points[i];
        GPPtr<Point> pointwrap;
        switch (p->type)
        {
            case GP__DAGPOINT__TYPE__FUNCTION:
            {
                GPPtr<GPComputePoint> cppoint = new GPComputePoint(database->vQueryFunctionByShortName(p->function));
                auto cp = new CP(cppoint);
                pointwrap = cp;
                mFunctions.push_back(cp);
            }
                break;
            case GP__DAGPOINT__TYPE__INPUT:
            {
                pointwrap = new SP;
                inputPoints.insert(std::make_pair(p->position, pointwrap));
            }
                break;
            case GP__DAGPOINT__TYPE__TRANSMIT:
            {
                pointwrap = new TP;
            }
                break;
            case GP__DAGPOINT__TYPE__OUTPUT:
            {
                pointwrap = new DP;
                outputPoints.insert(std::make_pair(p->position, pointwrap));
            }
                break;
            default:
                break;
        }
        GPASSERT(NULL!=pointwrap.get());
        pointmap.insert(std::make_pair(p->id, pointwrap));
    }
    for (int i=0; i<dag->n_connections; ++i)
    {
        auto connection = dag->connections[i];
        GPASSERT(pointmap.find(connection->dstpoint)!=pointmap.end());
        GPASSERT(pointmap.find(connection->srcpoint)!=pointmap.end());
        Point* src = pointmap.find(connection->srcpoint)->second.get();
        GPPtr<Point> dst = pointmap.find(connection->dstpoint)->second;
        src->connectOutput(dst, connection->get_src);
        dst->connectInput(src, connection->put_dst);
    }
    
    mSources.resize(inputPoints.size());
    for (auto iter : inputPoints)
    {
        GPASSERT(iter.first < mSources.size());
        mSources[iter.first] = iter.second;
    }
    
    mDest.resize(outputPoints.size());
    for (auto iter : outputPoints)
    {
        GPASSERT(iter.first < mDest.size());
        mDest[iter.first] = iter.second;
    }
}
GPDAGFunction::~GPDAGFunction()
{
}

GPContents* GPDAGFunction::vRun(GPContents* inputs)
{
    GPASSERT(NULL!=inputs);
    GPASSERT(inputs->size() == mSources.size());
    for (int i=0; i<mSources.size(); ++i)
    {
        CONTENT c(inputs->getContent(i));
        mSources[i]->vReceive(c, NULL);
    }
    GPContents* dst = new GPContents;
    for (int i=0; i<mDest.size(); ++i)
    {
        auto d = GPFORCECONVERT(DP, mDest[i].get());
        dst->pushContent(d->get());
    }
    return dst;
}