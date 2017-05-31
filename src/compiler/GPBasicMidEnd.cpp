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
#include <memory>
#include <string.h>
#include "utils/AutoStorage.h"

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

static std::function<void(GP__Point* message)> gFreePoint = [](GP__Point* message){
    gp__point__free_unpacked(message, NULL);
};

static GP__Point* _copyPoint(const GP__Point* formula)
{
    auto size = gp__point__get_packed_size(formula);
    AUTOSTORAGE(content, uint8_t, (int)size);
    gp__point__pack(formula, content);
    return gp__point__unpack(NULL, size, content);
}

static void _replaceVariable(GP__Point* dst, const std::map<int, std::shared_ptr<GP__Point>>& inputmap)
{
    GPASSERT(NULL!=dst);
    for (int i=0; i<dst->n_input_variable; ++i)
    {
        auto p = dst->input_variable[i];
        if (GP__POINT__TYPE__INPUT == p->type)
        {
            auto number = getNumber(p->content);
            GPASSERT(inputmap.end()!=inputmap.find(number));
            gFreePoint(p);
            dst->input_variable[i] = _copyPoint(inputmap.find(number)->second.get());
            continue;
        }
        _replaceVariable(p, inputmap);
    }
}

static void _replaced(GP__Point* dst, const GP__Point* adf, const char* name)
{
    GPASSERT(NULL!=dst);
    GPASSERT(NULL!=adf);
    if (GP__POINT__TYPE__FUNCTION == dst->type && 0 == ::strcmp(dst->content, name))
    {
        std::map<int, std::shared_ptr<GP__Point>> inputmap;
        for (int i=0; i<dst->n_input_variable; ++i)
        {
            auto p = _copyPoint(dst->input_variable[i]);
            inputmap.insert(std::make_pair(i, std::shared_ptr<GP__Point>(p, gFreePoint)));
        }
        auto copyed_adf = _copyPoint(adf);
        GPASSERT(GP__POINT__TYPE__FUNCTION == copyed_adf->type);
        _replaceVariable(copyed_adf, inputmap);
        
        //Replace the child of dst by copyed_adf's child
        for (int i=0; i<dst->n_input_variable; ++i)
        {
            gFreePoint(dst->input_variable[i]);
        }
        ::free(dst->input_variable);
        dst->input_variable = copyed_adf->input_variable;
        dst->n_input_variable = copyed_adf->n_input_variable;
        copyed_adf->n_input_variable = 0;
        copyed_adf->input_variable = NULL;
        
        //copy the content of copyed_adf
        ::free(dst->content);
        dst->content = ::strdup(copyed_adf->content);
        
        gFreePoint(copyed_adf);
    }
    for (int i=0; i<dst->n_input_variable; ++i)
    {
        _replaced(dst->input_variable[i], adf, name);
    }
}

static std::shared_ptr<GP__Point> _convertFormula(const GP__Point* formula, const GP__PointGroup* background)
{
    GPASSERT(NULL!=formula);
    GPASSERT(NULL!=background);
    auto treated = std::shared_ptr<GP__Point>(_copyPoint(formula), gFreePoint);

    //Search ADF Function and replace
    for (int i=0; i<background->n_adfs; ++i)
    {
        auto adf = background->adfs[i];
        GPASSERT(NULL!=adf->realization);
        GPASSERT(treated->type!=GP__POINT__TYPE__FUNCTION);
        _replaced(treated.get(), adf->realization, adf->name);
    }
    return treated;
}

GP__DAG* GPBasicMidEnd::vCreate(const GP__PointGroup* front) const
{
    //Get All Points
    std::vector<std::shared_ptr<GP__Point>> copyFormulas;
    for (int i=0; i<front->n_formulas; ++i)
    {
        copyFormulas.push_back(_convertFormula(front->formulas[i], front));
    }
    std::queue<GP__Point*> workqueue;
    for (int i=0; i<copyFormulas.size(); ++i)
    {
        workqueue.push(copyFormulas[i].get());
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
                    dag_point->has_position = true;
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
            dag_point->has_position = true;
            
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
    GPASSERT(workqueue.empty());
    for (int i=0; i<copyFormulas.size(); ++i)
    {
        workqueue.push(copyFormulas[i].get());
    }
    while (!workqueue.empty())
    {
        auto current = workqueue.front();
        workqueue.pop();
        GPASSERT(originPoints.find(current)!=originPoints.end());
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
