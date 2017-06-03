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
bool GPDAGFunction::SP::vReceive(CONTENT con, int put_dst)
{
    GPASSERT(0==put_dst);
    GPASSERT(NULL!=con.get());
    GPASSERT(NULL!=con->type());
    GPASSERT(NULL!=con->content());
    for (auto& o : mOutputs)
    {
        o.second.first->vReceive(con, o.second.second);
    }
    return true;
}

bool GPDAGFunction::CP::vReceive(CONTENT c, int put_dst)
{
    GPASSERT(put_dst>=0);
    if (mPoint->receive(c, put_dst))
    {
        auto comp = mPoint->compute();
        for (auto& o : mOutputs)
        {
            o.second.first->vReceive(comp->getContent(o.first), o.second.second);
        }
        delete comp;
    }
    return true;
}


GPDAGFunction::DP::DP():Point(1, 0)
{
    mContents = NULL;
}


bool GPDAGFunction::DP::vReceive(CONTENT c, int put_dst)
{
    GPASSERT(0 == put_dst);
    GPASSERT(NULL == mContents.get());
    mContents = c;
    return false;
}


void GPDAGFunction::DP::reset()
{
    mContents = NULL;
}

bool GPDAGFunction::TP::vReceive(CONTENT c, int put_dst)
{
    GPASSERT(0 <= put_dst && put_dst < mOutputs.size());
    for (auto &o : mOutputs)
    {
        if (o.first == put_dst)
        {
            o.second.first->vReceive(c, o.second.second);
        }
    }
    return true;
}


GPDAGFunction::GPDAGFunction(const GP__DAG* dag, const GPFunctionDataBase* database)
{
    std::map<uint32_t, GPPtr<Point>> pointmap;
    std::map<uint32_t, GPPtr<Point>> outputPoints;
    for (int i=0; i<dag->n_points; ++i)
    {
        auto p = dag->points[i];
        GPPtr<Point> pointwrap;
        switch (p->type)
        {
            case GP__DAGPOINT__TYPE__FUNCTION:
            {
                auto f = database->vQueryFunctionByShortName(p->function);
                if (NULL == f)
                {
                    FUNC_PRINT_ALL(p->function, s);
                    GPASSERT(false);
                }
                GPPtr<GPComputePoint> cppoint = new GPComputePoint(f);
                auto cp = new CP(cppoint);
                mComputePoints.push_back(cppoint.get());
                pointwrap = cp;
            }
                break;
            case GP__DAGPOINT__TYPE__INPUT:
            {
                pointwrap = new SP;
                mSources.push_back(std::make_pair(p->position, pointwrap));
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
        src->connectOutput(dst, connection->get_src, connection->put_dst);
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
    for (int i=0; i<mDest.size(); ++i)
    {
        auto d = GPFORCECONVERT(DP, mDest[i].get());
        d->reset();
    }
    for (auto s : mSources)
    {
        s.second->vReceive(inputs->getContent(s.first), 0);
    }
    GPContents* dst = new GPContents;
    for (int i=0; i<mDest.size(); ++i)
    {
        auto d = GPFORCECONVERT(DP, mDest[i].get());
        dst->pushContent(d->get());
    }
    return dst;
}

size_t GPDAGFunction::vMapParameters(const double* p, size_t n)
{
    int sum = 0;
    std::vector<int> numbers;
    for (auto c : mComputePoints)
    {
        auto number = c->map(NULL, 0);
        sum += number;
        numbers.push_back(number);
    }
    if (NULL == p)
    {
        return sum;
    }
    GPASSERT(n == sum);
    int offset = 0;
    for (int i=0; i<mComputePoints.size(); ++i)
    {
        mComputePoints[i]->map(p+offset, numbers[i]);
        offset += numbers[i];
    }
    return sum;
}
