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
#include "GPBasicMidEnd.h"
#include "utils/GPDebug.h"
#include <stdlib.h>
#include <string.h>
#include <queue>
#include <map>

static int getNumber(const std::string& name)
{
    GPASSERT(!name.empty());
    int p = 0;
    for (int i=1; i<name.size(); ++i)
    {
        p = p*10 + (name[i]-'0');
    }
    return p;
}

GP__DAG* GPBasicMidEnd::vCreate(const GP__PointGroup* front) const
{
    //Get All Points
    std::queue<GP__Point*> workqueue;
    for (int i=0; i<front->n_formulas; ++i)
    {
        workqueue.push(front->formulas[i]);
    }
    std::vector<GP__DAGPoint*> points;
    std::map<GP__Point*, uint32_t> originPoints;
    std::vector<GP__Point*> inputPoints;
    std::vector<GP__Point*> outputPoints;
    uint32_t index = 0;
    while (!workqueue.empty())
    {
        auto current = workqueue.front();
        workqueue.pop();
        for (int i=0; i<current->n_input_variable; ++i)
        {
            workqueue.push(current->input_variable[i]);
        }
        GP__DAGPoint* dag_point = (GP__DAGPoint*)::malloc(sizeof(GP__DAGPoint));
        gp__dagpoint__init(dag_point);
        dag_point->id = index++;
        points.push_back(dag_point);
        originPoints.insert(std::make_pair(current, dag_point->id));
        switch (current->type)
        {
            case GP__POINT__TYPE__FUNCTION:
                dag_point->type = GP__DAGPOINT__TYPE__FUNCTION;
                dag_point->function = ::strdup(current->content);
                break;
            case GP__POINT__TYPE__INPUT:
            {
                std::string key = current->content;
                if (key[0]=='x')
                {
                    dag_point->type = GP__DAGPOINT__TYPE__INPUT;
                    dag_point->position = getNumber(key);
                }
                else
                {
                    inputPoints.push_back(current);
                    dag_point->type = GP__DAGPOINT__TYPE__TRANSMIT;
                }
            }
                break;
            case GP__POINT__TYPE__OUTPUT:
                outputPoints.push_back(current);
                dag_point->type = GP__DAGPOINT__TYPE__TRANSMIT;
                break;
            default:
                GPASSERT(false);
                break;
        }
    }
    
    std::vector<GP__DAG__Connection*> connections;
    //Connect Transmit points
    for (auto input : inputPoints)
    {
        auto dstId = originPoints.find(input)->second;
        GP__Point* matchPoint = NULL;
        uint32_t matchPos = -1;
        for (auto output : outputPoints)
        {
            for (int i=0; i<output->n_output_names; ++i)
            {
                if (0 == ::strcmp(input->content, output->output_names[i]))
                {
                    matchPoint = output;
                    matchPos = i;
                    break;
                }
            }
            if (NULL!=matchPoint)
            {
                break;
            }
        }
        GP__DAG__Connection* conn = (GP__DAG__Connection*)malloc(sizeof(GP__DAG__Connection));
        gp__dag__connection__init(conn);
        connections.push_back(conn);
        conn->dstpoint = dstId;
        conn->srcpoint = originPoints.find(matchPoint)->second;
        conn->put_dst = 0;
        conn->get_src = matchPos;
    }

    //Create Output Points and Connect it
    for (auto output : outputPoints)
    {
        auto srcId = originPoints.find(output)->second;
        for (int i=0; i<output->n_output_names; ++i)
        {
            auto key = output->output_names[i];
            if (key[0] != 'y')
            {
                continue;
            }
            GP__DAGPoint* dag_point = (GP__DAGPoint*)::malloc(sizeof(GP__DAGPoint));
            gp__dagpoint__init(dag_point);
            dag_point->id = index++;
            points.push_back(dag_point);
            dag_point->type = GP__DAGPOINT__TYPE__OUTPUT;
            dag_point->position = getNumber(key);
            
            GP__DAG__Connection* conn = (GP__DAG__Connection*)malloc(sizeof(GP__DAG__Connection));
            gp__dag__connection__init(conn);
            connections.push_back(conn);
            conn->dstpoint = dag_point->id;
            conn->srcpoint = srcId;
            conn->put_dst = 0;
            conn->get_src = i;
        }
    }
    
    //Connect Parent and Children
    for (int i=0; i<front->n_formulas; ++i)
    {
        workqueue.push(front->formulas[i]);
    }
    while (!workqueue.empty())
    {
        auto current = workqueue.front();
        workqueue.pop();
        uint32_t dstId = originPoints.find(current)->second;
        for (int i=0; i<current->n_input_variable; ++i)
        {
            workqueue.push(current->input_variable[i]);
        }
        if (GP__POINT__TYPE__OUTPUT == current->type)
        {
            GPASSERT(current->n_input_variable == 1);
            uint32_t srcId = originPoints.find(current->input_variable[0])->second;
            for (int i=0; i<current->n_output_names; ++i)
            {
                GP__DAG__Connection* conn = (GP__DAG__Connection*)malloc(sizeof(GP__DAG__Connection));
                gp__dag__connection__init(conn);
                connections.push_back(conn);
                conn->dstpoint = dstId;
                conn->srcpoint = srcId;
                conn->put_dst = i;
                conn->get_src = i;
            }
            continue;
        }
        
        for (int i=0; i<current->n_input_variable; ++i)
        {
            uint32_t srcId = originPoints.find(current->input_variable[i])->second;
            GP__DAG__Connection* conn = (GP__DAG__Connection*)malloc(sizeof(GP__DAG__Connection));
            gp__dag__connection__init(conn);
            connections.push_back(conn);
            conn->dstpoint = dstId;
            conn->srcpoint = srcId;
            conn->put_dst = i;
            conn->get_src = 0;
        }
    }

    //Copy Result
    auto dag = (GP__DAG*)::malloc(sizeof(GP__DAG));
    gp__dag__init(dag);
    dag->n_points = points.size();
    dag->n_connections = connections.size();
    dag->points = (GP__DAGPoint**)::malloc(points.size()*sizeof(GP__DAGPoint*));
    ::memcpy(dag->points, points.data(), points.size()*sizeof(GP__DAGPoint*));
    
    dag->connections = (GP__DAG__Connection**)::malloc(connections.size()*sizeof(GP__DAG__Connection*));
    ::memcpy(dag->connections, connections.data(), connections.size()*sizeof(GP__DAG__Connection*));
    return dag;
}
